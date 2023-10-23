#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
/*
 * MutationLogReader.actor.h
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

#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_MUTATION_LOG_READER_ACTOR_G_H)
#define FDBCLIENT_MUTATION_LOG_READER_ACTOR_G_H
#include "fdbclient/MutationLogReader.actor.g.h"
#elif !defined(FDBCLIENT_MUTATION_LOG_READER_ACTOR_H)
#define FDBCLIENT_MUTATION_LOG_READER_ACTOR_H

#include <deque>
#include "fdbclient/FDBTypes.h"
#include "fdbclient/NativeAPI.actor.h"
#include "flow/flow.h"
#include "flow/ActorCollection.h"
#include "flow/actorcompiler.h" // has to be last include

namespace mutation_log_reader {

// RangeResultBlock is the wrapper of RangeResult. Each PipelinedReader maintains a Deque of RangeResultBlocks, from its
// getRange results. MutationLogReader maintains a min heap of RangeResultBlocks, and provides RangeResult
// partially in it to consumer.
struct RangeResultBlock {
	RangeResult result;
	Version firstVersion; // version of first record, inclusive
	Version lastVersion; // version of last record, inclusive
	uint8_t hash; // points back to the PipelinedReader
	int prefixLen; // size of keyspace, uid, and hash prefix
	int indexToRead; // index of first unconsumed record

	// When the consumer reads, provides (partial) RangeResult, from firstVersion to min(lastVersion, firstVersion
	// rounded up to the nearest 1M), to ensure that no versions out of this RangeResultBlock can be in between.
	Standalone<RangeResultRef> consume();

	bool empty() { return indexToRead == result.size(); }

	bool operator<(const RangeResultBlock& r) const {
		// We want a min heap. The standard C++ priority queue is a max heap.
		return firstVersion > r.firstVersion;
	}
};

// PipelinedReader is the class actually doing range read (getRange). A MutationLogReader has 256 PipelinedReaders, each
// in charge of one hash value from 0-255.
class PipelinedReader {
public:
	PipelinedReader(uint8_t h, Version bv, Version ev, unsigned pd, Key p)
	  : readerLimit(pd), hash(h), prefix(StringRef(&hash, sizeof(uint8_t)).withPrefix(p)), beginVersion(bv),
	    endVersion(ev), currentBeginVersion(bv), pipelineDepth(pd) {}

	void startReading(Database cx);
	Future<Void> getNext(Database cx);
																#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
[[nodiscard]] static Future<Void> getNext_impl( PipelinedReader* const& self, Database const& cx );
template <class> friend class PipelinedReader_GetNext_implActorState;

#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"

	void release() { readerLimit.release(); }

	PromiseStream<RangeResultBlock> reads;
	FlowLock readerLimit;
	uint8_t hash;
	Key prefix; // "\xff\x02/alog/UID/hash/" for restore, or "\xff\x02/blog/UID/hash/" for backup

	Future<Void> done() { return reader; }

private:
	Version beginVersion, endVersion, currentBeginVersion;
	unsigned pipelineDepth;
	Future<Void> reader;
};

} // namespace mutation_log_reader

// MutationLogReader provides a strictly version ordered stream of KV pairs that represent mutation log chunks written
// by the FDB backup log feature. A MutationLogReader has 256 PipelinedReaders, each in charge of one hash value from
// 0-255. It keeps a min heap of RangeResultBlocks, ordered by their first version. At any time, each PipelinedReader
// has at most one RangeResultBlock in MutationLogReader's min heap. When the consumer reads from MutationLogReader, the
// MutationLogReader calls the heap's top RangeResultBlock's consume() function, to make sure it does deliver perfectly
// ordered mutations.
class MutationLogReader : public ReferenceCounted<MutationLogReader> {
public:
	MutationLogReader() : finished(256) {}
	MutationLogReader(Database cx, Version bv, Version ev, Key uid, Key beginKey, unsigned pd)
	  : beginVersion(bv), endVersion(ev), prefix(uid.withPrefix(beginKey)), pipelineDepth(pd), finished(0) {
		pipelinedReaders.reserve(256);
		if (pipelineDepth > 0) {
			for (int h = 0; h < 256; ++h) {
				pipelinedReaders.emplace_back(new mutation_log_reader::PipelinedReader(
				    (uint8_t)h, beginVersion, endVersion, pipelineDepth, prefix));
				pipelinedReaders[h]->startReading(cx);
			}
		}
	}

																#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
// This generated class is to be used only via Create()
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
template <class CreateActor>
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
class CreateActorState {
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
public:
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	CreateActorState(Database const& cx,Version const& bv,Version const& ev,Key const& uid,Key const& beginKey,unsigned const& pd) 
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		 : cx(cx),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		   bv(bv),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		   ev(ev),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		   uid(uid),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		   beginKey(beginKey),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		   pd(pd),
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		   self(new MutationLogReader(cx, bv, ev, uid, beginKey, pd))
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
	{
		fdb_probe_actor_create("Create", reinterpret_cast<unsigned long>(this));

	}
	~CreateActorState() 
	{
		fdb_probe_actor_destroy("Create", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
			StrictFuture<Void> __when_expr_0 = self->initializePQ(self.getPtr());
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
			if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateActor*>(this)->actor_wait_state = 1;
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 0, Void >*>(static_cast<CreateActor*>(this)));
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
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
		this->~CreateActorState();
		static_cast<CreateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		if (!static_cast<CreateActor*>(this)->SAV<Reference<MutationLogReader>>::futures) { (void)(self); this->~CreateActorState(); static_cast<CreateActor*>(this)->destroy(); return 0; }
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
		new (&static_cast<CreateActor*>(this)->SAV< Reference<MutationLogReader> >::value()) Reference<MutationLogReader>(std::move(self)); // state_var_RVO
		this->~CreateActorState();
		static_cast<CreateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
		if (!static_cast<CreateActor*>(this)->SAV<Reference<MutationLogReader>>::futures) { (void)(self); this->~CreateActorState(); static_cast<CreateActor*>(this)->destroy(); return 0; }
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
		new (&static_cast<CreateActor*>(this)->SAV< Reference<MutationLogReader> >::value()) Reference<MutationLogReader>(std::move(self)); // state_var_RVO
		this->~CreateActorState();
		static_cast<CreateActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<CreateActor*>(this)->actor_wait_state > 0) static_cast<CreateActor*>(this)->actor_wait_state = 0;
		static_cast<CreateActor*>(this)->ActorCallback< CreateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("Create", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("Create", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("Create", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("Create", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("Create", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("Create", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	Database cx;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	Version bv;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	Version ev;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	Key uid;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	Key beginKey;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	unsigned pd;
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	Reference<MutationLogReader> self;
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
};
// This generated class is to be used only via Create()
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
class CreateActor final : public Actor<Reference<MutationLogReader>>, public ActorCallback< CreateActor, 0, Void >, public FastAllocated<CreateActor>, public CreateActorState<CreateActor> {
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
public:
	using FastAllocated<CreateActor>::operator new;
	using FastAllocated<CreateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<MutationLogReader>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateActor, 0, Void >;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	CreateActor(Database const& cx,Version const& bv,Version const& ev,Key const& uid,Key const& beginKey,unsigned const& pd) 
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
		 : Actor<Reference<MutationLogReader>>(),
		   CreateActorState<CreateActor>(cx, bv, ev, uid, beginKey, pd)
	{
		fdb_probe_actor_enter("Create", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("Create");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("Create", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
[[nodiscard]] static Future<Reference<MutationLogReader>> Create( Database const& cx, Version const& bv, Version const& ev, Key const& uid, Key const& beginKey, unsigned const& pd ) {
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
	return Future<Reference<MutationLogReader>>(new CreateActor(cx, bv, ev, uid, beginKey, pd));
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
}

#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"

	Future<Standalone<RangeResultRef>> getNext();

private:
																#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
[[nodiscard]] static Future<Void> initializePQ( MutationLogReader* const& self );
template <class> friend class MutationLogReader_InitializePQActorState;

#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"
																#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.g.h"
[[nodiscard]] static Future<Standalone<RangeResultRef>> getNext_impl( MutationLogReader* const& self );
template <class> friend class MutationLogReader_GetNext_implActorState;

#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MutationLogReader.actor.h"

	std::vector<std::unique_ptr<mutation_log_reader::PipelinedReader>> pipelinedReaders;
	std::priority_queue<mutation_log_reader::RangeResultBlock> priorityQueue;
	Version beginVersion, endVersion;
	Key prefix; // "\xff\x02/alog/UID/" for restore, or "\xff\x02/blog/UID/" for backup
	unsigned pipelineDepth;
	unsigned finished;
};

#include "flow/unactorcompiler.h"
#endif
