#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
/*
 * AsyncFileCached.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FLOW_ASYNCFILECACHED_ACTOR_G_H)
#define FLOW_ASYNCFILECACHED_ACTOR_G_H
#include "fdbrpc/AsyncFileCached.actor.g.h"
#elif !defined(FLOW_ASYNCFILECACHED_ACTOR_H)
#define FLOW_ASYNCFILECACHED_ACTOR_H

#include <boost/intrusive/list.hpp>
#include <type_traits>

#include "flow/flow.h"
#include "flow/IAsyncFile.h"
#include "flow/Knobs.h"
#include "flow/TDMetric.actor.h"
#include "flow/network.h"
#include "flow/actorcompiler.h" // This must be the last #include.

namespace bi = boost::intrusive;
struct EvictablePage {
	void* data;
	int index;
	class Reference<struct EvictablePageCache> pageCache;
	bi::list_member_hook<> member_hook;

	virtual bool evict() = 0; // true if page was evicted, false if it isn't immediately evictable (but will be evicted
	                          // regardless if possible)

	EvictablePage(Reference<EvictablePageCache> pageCache) : data(0), index(-1), pageCache(pageCache) {}
	virtual ~EvictablePage();
};

struct EvictablePageCache : ReferenceCounted<EvictablePageCache> {
	using List =
	    bi::list<EvictablePage, bi::member_hook<EvictablePage, bi::list_member_hook<>, &EvictablePage::member_hook>>;
	enum CacheEvictionType { RANDOM = 0, LRU = 1 };

	static CacheEvictionType evictionPolicyStringToEnum(const std::string& policy) {
		std::string cep = policy;
		std::transform(cep.begin(), cep.end(), cep.begin(), ::tolower);
		if (cep != "random" && cep != "lru")
			throw invalid_cache_eviction_policy();

		if (cep == "random")
			return RANDOM;
		return LRU;
	}

	EvictablePageCache() : pageSize(0), maxPages(0), cacheEvictionType(RANDOM) {}

	explicit EvictablePageCache(int pageSize, int64_t maxSize)
	  : pageSize(pageSize), maxPages(maxSize / pageSize),
	    cacheEvictionType(evictionPolicyStringToEnum(FLOW_KNOBS->CACHE_EVICTION_POLICY)) {
		cacheEvictions.init("EvictablePageCache.CacheEvictions"_sr);
	}

	void allocate(EvictablePage* page) {
		try_evict();
		try_evict();

		page->data = allocateFast4kAligned(pageSize);

		if (RANDOM == cacheEvictionType) {
			page->index = pages.size();
			pages.push_back(page);
		} else {
			lruPages.push_back(*page); // new page is considered the most recently used (placed at LRU tail)
		}
	}

	void updateHit(EvictablePage* page) {
		if (RANDOM != cacheEvictionType) {
			// on a hit, update page's location in the LRU so that it's most recent (tail)
			lruPages.erase(List::s_iterator_to(*page));
			lruPages.push_back(*page);
		}
	}

	void try_evict() {
		if (RANDOM == cacheEvictionType) {
			if (pages.size() >= (uint64_t)maxPages && !pages.empty()) {
				for (int i = 0; i < FLOW_KNOBS->MAX_EVICT_ATTEMPTS;
				     i++) { // If we don't manage to evict anything, just go ahead and exceed the cache limit
					int toEvict = deterministicRandom()->randomInt(0, pages.size());
					if (pages[toEvict]->evict()) {
						++cacheEvictions;
						break;
					}
				}
			}
		} else {
			// For now, LRU is the only other CACHE_EVICTION option
			if (lruPages.size() >= (uint64_t)maxPages) {
				int i = 0;
				// try the least recently used pages first (starting at head of the LRU list)
				for (List::iterator it = lruPages.begin(); it != lruPages.end() && i < FLOW_KNOBS->MAX_EVICT_ATTEMPTS;
				     ++it, ++i) { // If we don't manage to evict anything, just go ahead and exceed the cache limit
					if (it->evict()) {
						++cacheEvictions;
						break;
					}
				}
			}
		}
	}

	std::vector<EvictablePage*> pages;
	List lruPages;
	int pageSize;
	int64_t maxPages;
	Int64MetricHandle cacheEvictions;
	const CacheEvictionType cacheEvictionType;
};

struct AFCPage;

class AsyncFileCached final : public IAsyncFile, public ReferenceCounted<AsyncFileCached> {
	friend struct AFCPage;

public:
	// Opens a file that uses the FDB in-memory page cache
	static Future<Reference<IAsyncFile>> open(std::string filename, int flags, int mode) {
		//TraceEvent("AsyncFileCachedOpen").detail("Filename", filename);
		auto itr = openFiles.find(filename);
		if (itr == openFiles.end()) {
			auto f = open_impl(filename, flags, mode);
			if (f.isReady() && f.isError())
				return f;

			auto result = openFiles.try_emplace(filename, f);

			// This should be inserting a new entry
			ASSERT(result.second);
			itr = result.first;

			// We return here instead of falling through to the outer scope so that we don't delete all references to
			// the underlying file before returning
			return itr->second.get();
		}
		return itr->second.get();
	}

	Future<int> read(void* data, int length, int64_t offset) override {
		++countFileCacheReads;
		++countCacheReads;
		if (offset + length > this->length) {
			length = int(this->length - offset);
			ASSERT(length >= 0);
		}
		auto f = read_write_impl<false>(this, static_cast<uint8_t*>(data), length, offset);
		if (f.isReady() && !f.isError())
			return length;
		++countFileCacheReadsBlocked;
		++countCacheReadsBlocked;
		return tag(f, length);
	}

																#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via write_impl()
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class Write_implActor>
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Write_implActorState {
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Write_implActorState(AsyncFileCached* const& self,void const* const& data,int const& length,int64_t const& offset) 
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self),
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   data(data),
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   length(length),
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   offset(offset)
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("write_impl", reinterpret_cast<unsigned long>(this));

	}
	~Write_implActorState() 
	{
		fdb_probe_actor_destroy("write_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (length + offset > self->currentTruncateSize)
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				StrictFuture<Void> __when_expr_0 = self->currentTruncate;
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Write_implActor*>(this)->actor_wait_state = 1;
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 0, Void >*>(static_cast<Write_implActor*>(this)));
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		++self->countFileCacheWrites;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		++self->countCacheWrites;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		Future<Void> f = read_write_impl<true>(self, static_cast<const uint8_t*>(data), length, offset);
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!f.isReady())
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			++self->countFileCacheWritesBlocked;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			++self->countCacheWritesBlocked;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<Void> __when_expr_1 = f;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Write_implActor*>(this)->actor_wait_state = 2;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 1, Void >*>(static_cast<Write_implActor*>(this)));
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

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
		if (static_cast<Write_implActor*>(this)->actor_wait_state > 0) static_cast<Write_implActor*>(this)->actor_wait_state = 0;
		static_cast<Write_implActor*>(this)->ActorCallback< Write_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Write_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Write_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Write_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<Write_implActor*>(this)->actor_wait_state > 0) static_cast<Write_implActor*>(this)->actor_wait_state = 0;
		static_cast<Write_implActor*>(this)->ActorCallback< Write_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Write_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Write_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Write_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AsyncFileCached* self;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	void const* data;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int length;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int64_t offset;
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via write_impl()
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Write_implActor final : public Actor<Void>, public ActorCallback< Write_implActor, 0, Void >, public ActorCallback< Write_implActor, 1, Void >, public FastAllocated<Write_implActor>, public Write_implActorState<Write_implActor> {
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<Write_implActor>::operator new;
	using FastAllocated<Write_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Write_implActor, 0, Void >;
friend struct ActorCallback< Write_implActor, 1, Void >;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Write_implActor(AsyncFileCached* const& self,void const* const& data,int const& length,int64_t const& offset) 
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   Write_implActorState<Write_implActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("write_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Write_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Write_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> write_impl( AsyncFileCached* const& self, void const* const& data, int const& length, int64_t const& offset ) {
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new Write_implActor(self, data, length, offset));
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	Future<Void> write(void const* data, int length, int64_t offset) override {
		return write_impl(this, data, length, offset);
	}

	Future<Void> readZeroCopy(void** data, int* length, int64_t offset) override;
	void releaseZeroCopy(void* data, int length, int64_t offset) override;

	// This waits for previously started truncates to finish and then truncates
	Future<Void> truncate(int64_t size) override { return truncate_impl(this, size); }

	// This is the 'real' truncate that does the actual removal of cache blocks and then shortens the file
	Future<Void> changeFileSize(int64_t size);

	// This wrapper for the actual truncation operation enforces ordering of truncates.
	// It maintains currentTruncate and currentTruncateSize so writers can wait behind truncates that would affect them.
																#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via truncate_impl()
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class Truncate_implActor>
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Truncate_implActorState {
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Truncate_implActorState(AsyncFileCached* const& self,int64_t const& size) 
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self),
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   size(size)
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	~Truncate_implActorState() 
	{
		fdb_probe_actor_destroy("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			StrictFuture<Void> __when_expr_0 = self->currentTruncate;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Truncate_implActor*>(this)->actor_wait_state = 1;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 0, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		self->currentTruncateSize = size;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		self->currentTruncate = self->changeFileSize(size);
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<Void> __when_expr_1 = self->currentTruncate;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Truncate_implActor*>(this)->actor_wait_state = 2;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 1, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		self->currentTruncateSize = size;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		self->currentTruncate = self->changeFileSize(size);
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<Void> __when_expr_1 = self->currentTruncate;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Truncate_implActor*>(this)->actor_wait_state = 2;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 1, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state > 0) static_cast<Truncate_implActor*>(this)->actor_wait_state = 0;
		static_cast<Truncate_implActor*>(this)->ActorCallback< Truncate_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Truncate_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Truncate_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActorState(); static_cast<Truncate_implActor*>(this)->destroy(); return 0; }
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Truncate_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Truncate_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActorState(); static_cast<Truncate_implActor*>(this)->destroy(); return 0; }
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Truncate_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state > 0) static_cast<Truncate_implActor*>(this)->actor_wait_state = 0;
		static_cast<Truncate_implActor*>(this)->ActorCallback< Truncate_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Truncate_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AsyncFileCached* self;
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int64_t size;
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via truncate_impl()
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Truncate_implActor final : public Actor<Void>, public ActorCallback< Truncate_implActor, 0, Void >, public ActorCallback< Truncate_implActor, 1, Void >, public FastAllocated<Truncate_implActor>, public Truncate_implActorState<Truncate_implActor> {
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<Truncate_implActor>::operator new;
	using FastAllocated<Truncate_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Truncate_implActor, 0, Void >;
friend struct ActorCallback< Truncate_implActor, 1, Void >;
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Truncate_implActor(AsyncFileCached* const& self,int64_t const& size) 
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   Truncate_implActorState<Truncate_implActor>(self, size)
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("truncate_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Truncate_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Truncate_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> truncate_impl( AsyncFileCached* const& self, int64_t const& size ) {
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new Truncate_implActor(self, size));
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	Future<Void> sync() override { return waitAndSync(this, flush()); }

	Future<int64_t> size() const override { return length; }

	int64_t debugFD() const override { return uncached->debugFD(); }

	std::string getFilename() const override { return filename; }

	void setRateControl(Reference<IRateControl> const& rc) override { rateControl = rc; }

	Reference<IRateControl> const& getRateControl() override { return rateControl; }

	void addref() override {
		ReferenceCounted<AsyncFileCached>::addref();
		//TraceEvent("AsyncFileCachedAddRef").detail("Filename", filename).detail("Refcount", debugGetReferenceCount()).backtrace();
	}
	void delref() override {
		if (delref_no_destroy()) {
			// If this is ever ThreadSafeReferenceCounted...
			// setrefCountUnsafe(0);

			if (rateControl) {
				TraceEvent(SevDebug, "AsyncFileCachedKillWaiters").detail("Filename", filename);
				rateControl->killWaiters(io_error());
			}

			auto f = quiesce();
			TraceEvent("AsyncFileCachedDel")
			    .detail("Filename", filename)
			    .detail("Refcount", debugGetReferenceCount())
			    .detail("CanDie", f.isReady());
			// .backtrace();
			if (f.isReady())
				delete this;
			else
				uncancellable(holdWhile(Reference<AsyncFileCached>::addRef(this), f));
		}
	}

	~AsyncFileCached() override;

private:
	// A map of filename to the file handle for all opened cached files
	static std::map<std::string, UnsafeWeakFutureReference<IAsyncFile>> openFiles;

	std::string filename;
	Reference<IAsyncFile> uncached;
	int64_t length;
	int64_t prevLength;
	std::unordered_map<int64_t, AFCPage*> pages;
	std::vector<AFCPage*> flushable;
	Reference<EvictablePageCache> pageCache;
	Future<Void> currentTruncate;
	int64_t currentTruncateSize;
	Reference<IRateControl> rateControl;

	// Map of pointers which hold page buffers for pages which have been overwritten
	// but at the time of write there were still readZeroCopy holders.
	std::unordered_map<void*, int> orphanedPages;

	Int64MetricHandle countFileCacheFinds;
	Int64MetricHandle countFileCacheReads;
	Int64MetricHandle countFileCacheWrites;
	Int64MetricHandle countFileCacheReadsBlocked;
	Int64MetricHandle countFileCacheWritesBlocked;
	Int64MetricHandle countFileCachePageReadsHit;
	Int64MetricHandle countFileCachePageReadsMissed;
	Int64MetricHandle countFileCachePageReadsMerged;
	Int64MetricHandle countFileCacheReadBytes;

	Int64MetricHandle countCacheFinds;
	Int64MetricHandle countCacheReads;
	Int64MetricHandle countCacheWrites;
	Int64MetricHandle countCacheReadsBlocked;
	Int64MetricHandle countCacheWritesBlocked;
	Int64MetricHandle countCachePageReadsHit;
	Int64MetricHandle countCachePageReadsMissed;
	Int64MetricHandle countCachePageReadsMerged;
	Int64MetricHandle countCacheReadBytes;

	AsyncFileCached(Reference<IAsyncFile> uncached,
	                const std::string& filename,
	                int64_t length,
	                Reference<EvictablePageCache> pageCache)
	  : filename(filename), uncached(uncached), length(length), prevLength(length), pageCache(pageCache),
	    currentTruncate(Void()), currentTruncateSize(0), rateControl(nullptr) {
		if (!g_network->isSimulated()) {
			countFileCacheWrites.init("AsyncFile.CountFileCacheWrites"_sr, filename);
			countFileCacheReads.init("AsyncFile.CountFileCacheReads"_sr, filename);
			countFileCacheWritesBlocked.init("AsyncFile.CountFileCacheWritesBlocked"_sr, filename);
			countFileCacheReadsBlocked.init("AsyncFile.CountFileCacheReadsBlocked"_sr, filename);
			countFileCachePageReadsHit.init("AsyncFile.CountFileCachePageReadsHit"_sr, filename);
			countFileCachePageReadsMissed.init("AsyncFile.CountFileCachePageReadsMissed"_sr, filename);
			countFileCachePageReadsMerged.init("AsyncFile.CountFileCachePageReadsMerged"_sr, filename);
			countFileCacheFinds.init("AsyncFile.CountFileCacheFinds"_sr, filename);
			countFileCacheReadBytes.init("AsyncFile.CountFileCacheReadBytes"_sr, filename);

			countCacheWrites.init("AsyncFile.CountCacheWrites"_sr);
			countCacheReads.init("AsyncFile.CountCacheReads"_sr);
			countCacheWritesBlocked.init("AsyncFile.CountCacheWritesBlocked"_sr);
			countCacheReadsBlocked.init("AsyncFile.CountCacheReadsBlocked"_sr);
			countCachePageReadsHit.init("AsyncFile.CountCachePageReadsHit"_sr);
			countCachePageReadsMissed.init("AsyncFile.CountCachePageReadsMissed"_sr);
			countCachePageReadsMerged.init("AsyncFile.CountCachePageReadsMerged"_sr);
			countCacheFinds.init("AsyncFile.CountCacheFinds"_sr);
			countCacheReadBytes.init("AsyncFile.CountCacheReadBytes"_sr);
		}
	}

	static Future<Reference<IAsyncFile>> open_impl(std::string filename, int flags, int mode);

	// Opens a file that uses the FDB in-memory page cache
																#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via open_impl()
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class Open_implActor>
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Open_implActorState {
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Open_implActorState(std::string const& filename,int const& flags,int const& mode,Reference<EvictablePageCache> const& pageCache) 
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : filename(filename),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   flags(flags),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   mode(mode),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   pageCache(pageCache)
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("open_impl", reinterpret_cast<unsigned long>(this));

	}
	~Open_implActorState() 
	{
		fdb_probe_actor_destroy("open_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				TraceEvent("AFCUnderlyingOpenBegin").detail("Filename", filename);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (flags & IAsyncFile::OPEN_CACHED_READ_ONLY)
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					flags = (flags & ~IAsyncFile::OPEN_READWRITE) | IAsyncFile::OPEN_READONLY;
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				}
				else
				{
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					flags = (flags & ~IAsyncFile::OPEN_READONLY) | IAsyncFile::OPEN_READWRITE;
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				}
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, flags | IAsyncFile::OPEN_UNCACHED | IAsyncFile::OPEN_UNBUFFERED, mode);
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (static_cast<Open_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Open_implActor*>(this)->actor_wait_state = 1;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >*>(static_cast<Open_implActor*>(this)));
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~Open_implActorState();
		static_cast<Open_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (e.code() != error_code_actor_cancelled)
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				openFiles.erase(filename);
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			}
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		TraceEvent("AFCUnderlyingOpenEnd").detail("Filename", filename);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<int64_t> __when_expr_1 = f->size();
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<Open_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<Open_implActor*>(this)->actor_wait_state = 2;
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Open_implActor, 1, int64_t >*>(static_cast<Open_implActor*>(this)));
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		f = __f;
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Open_implActor*>(this)->actor_wait_state > 0) static_cast<Open_implActor*>(this)->actor_wait_state = 0;
		static_cast<Open_implActor*>(this)->ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(int64_t const& l,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		TraceEvent("AFCUnderlyingSize").detail("Filename", filename).detail("Size", l);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Open_implActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(Reference<AsyncFileCached>(new AsyncFileCached(f, filename, l, pageCache)).castTo<IAsyncFile>()); this->~Open_implActorState(); static_cast<Open_implActor*>(this)->destroy(); return 0; }
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Open_implActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(Reference<AsyncFileCached>(new AsyncFileCached(f, filename, l, pageCache)).castTo<IAsyncFile>());
		this->~Open_implActorState();
		static_cast<Open_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(int64_t && l,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		TraceEvent("AFCUnderlyingSize").detail("Filename", filename).detail("Size", l);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Open_implActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(Reference<AsyncFileCached>(new AsyncFileCached(f, filename, l, pageCache)).castTo<IAsyncFile>()); this->~Open_implActorState(); static_cast<Open_implActor*>(this)->destroy(); return 0; }
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Open_implActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(Reference<AsyncFileCached>(new AsyncFileCached(f, filename, l, pageCache)).castTo<IAsyncFile>());
		this->~Open_implActorState();
		static_cast<Open_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(int64_t const& l,int loopDepth) 
	{
		loopDepth = a_body1cont5(l, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int64_t && l,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(l), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<Open_implActor*>(this)->actor_wait_state > 0) static_cast<Open_implActor*>(this)->actor_wait_state = 0;
		static_cast<Open_implActor*>(this)->ActorCallback< Open_implActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< Open_implActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Open_implActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Open_implActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	std::string filename;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int flags;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int mode;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Reference<EvictablePageCache> pageCache;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Reference<IAsyncFile> f;
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via open_impl()
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Open_implActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >, public ActorCallback< Open_implActor, 1, int64_t >, public FastAllocated<Open_implActor>, public Open_implActorState<Open_implActor> {
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<Open_implActor>::operator new;
	using FastAllocated<Open_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< Open_implActor, 1, int64_t >;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Open_implActor(std::string const& filename,int const& flags,int const& mode,Reference<EvictablePageCache> const& pageCache) 
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Reference<IAsyncFile>>(),
		   Open_implActorState<Open_implActor>(filename, flags, mode, pageCache)
	{
		fdb_probe_actor_enter("open_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("open_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("open_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Open_implActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Open_implActor, 1, int64_t >*)0, actor_cancelled()); break;
		}

	}
};
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Reference<IAsyncFile>> open_impl( std::string const& filename, int const& flags, int const& mode, Reference<EvictablePageCache> const& pageCache ) {
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Reference<IAsyncFile>>(new Open_implActor(filename, flags, mode, pageCache));
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	Future<Void> flush() override;

	Future<Void> quiesce();

																#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via waitAndSync()
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class WaitAndSyncActor>
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WaitAndSyncActorState {
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WaitAndSyncActorState(AsyncFileCached* const& self,Future<Void> const& flush) 
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self),
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   flush(flush)
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("waitAndSync", reinterpret_cast<unsigned long>(this));

	}
	~WaitAndSyncActorState() 
	{
		fdb_probe_actor_destroy("waitAndSync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			StrictFuture<Void> __when_expr_0 = flush;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (static_cast<WaitAndSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitAndSyncActor*>(this)->actor_wait_state = 1;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitAndSyncActor, 0, Void >*>(static_cast<WaitAndSyncActor*>(this)));
															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~WaitAndSyncActorState();
		static_cast<WaitAndSyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<Void> __when_expr_1 = self->uncached->sync();
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<WaitAndSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitAndSyncActor*>(this)->actor_wait_state = 2;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitAndSyncActor, 1, Void >*>(static_cast<WaitAndSyncActor*>(this)));
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<Void> __when_expr_1 = self->uncached->sync();
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<WaitAndSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitAndSyncActor*>(this)->actor_wait_state = 2;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitAndSyncActor, 1, Void >*>(static_cast<WaitAndSyncActor*>(this)));
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<WaitAndSyncActor*>(this)->actor_wait_state > 0) static_cast<WaitAndSyncActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndSyncActor*>(this)->ActorCallback< WaitAndSyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAndSyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitAndSyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitAndSyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WaitAndSyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndSyncActorState(); static_cast<WaitAndSyncActor*>(this)->destroy(); return 0; }
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WaitAndSyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndSyncActorState();
		static_cast<WaitAndSyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WaitAndSyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndSyncActorState(); static_cast<WaitAndSyncActor*>(this)->destroy(); return 0; }
															#line 1443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WaitAndSyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndSyncActorState();
		static_cast<WaitAndSyncActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<WaitAndSyncActor*>(this)->actor_wait_state > 0) static_cast<WaitAndSyncActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndSyncActor*>(this)->ActorCallback< WaitAndSyncActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAndSyncActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitAndSyncActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitAndSyncActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AsyncFileCached* self;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Future<Void> flush;
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via waitAndSync()
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WaitAndSyncActor final : public Actor<Void>, public ActorCallback< WaitAndSyncActor, 0, Void >, public ActorCallback< WaitAndSyncActor, 1, Void >, public FastAllocated<WaitAndSyncActor>, public WaitAndSyncActorState<WaitAndSyncActor> {
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<WaitAndSyncActor>::operator new;
	using FastAllocated<WaitAndSyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitAndSyncActor, 0, Void >;
friend struct ActorCallback< WaitAndSyncActor, 1, Void >;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WaitAndSyncActor(AsyncFileCached* const& self,Future<Void> const& flush) 
															#line 1535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   WaitAndSyncActorState<WaitAndSyncActor>(self, flush)
	{
		fdb_probe_actor_enter("waitAndSync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitAndSync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitAndSync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitAndSyncActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitAndSyncActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> waitAndSync( AsyncFileCached* const& self, Future<Void> const& flush ) {
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new WaitAndSyncActor(self, flush));
															#line 1563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	template <bool writing>
	static Future<Void> read_write_impl(AsyncFileCached* self,
	                                    typename std::conditional_t<writing, const uint8_t*, uint8_t*> data,
	                                    int length,
	                                    int64_t offset);

	void remove_page(AFCPage* page);
};

struct AFCPage : public EvictablePage, public FastAllocated<AFCPage> {
	bool evict() override {
		if (notReading.isReady() && notFlushing.isReady() && !dirty && !zeroCopyRefCount && !truncated) {
			owner->remove_page(this);
			delete this;
			return true;
		}

		if (dirty)
			flush();

		return false;
	}

	// Move this page's data into the orphanedPages set of the owner
	void orphan() {
		owner->orphanedPages[data] = zeroCopyRefCount;
		zeroCopyRefCount = 0;
		notReading = Void();
		data = allocateFast4kAligned(pageCache->pageSize);
	}

	Future<Void> write(void const* data, int length, int offset) {
		// If zero-copy reads are in progress, allow whole page writes to a new page buffer so the effects
		// are not seen by the prior readers who still hold zeroCopyRead pointers
		bool fullPage = offset == 0 && length == pageCache->pageSize;
		ASSERT(zeroCopyRefCount == 0 || fullPage);

		if (zeroCopyRefCount != 0) {
			ASSERT(fullPage);
			orphan();
		}

		setDirty();

		// If there are no active readers then if data is valid or we're replacing all of it we can write directly
		if (valid || fullPage) {
			if (!fullPage) {
				++owner->countFileCachePageReadsHit;
				++owner->countCachePageReadsHit;
			}
			valid = true;
			memcpy(static_cast<uint8_t*>(this->data) + offset, data, length);
			return yield();
		}

		++owner->countFileCachePageReadsMissed;
		++owner->countCachePageReadsMissed;

		// If data is not valid but no read is in progress, start reading
		if (notReading.isReady()) {
			notReading = readThrough(this);
		}

		notReading = waitAndWrite(this, data, length, offset);

		return notReading;
	}

																#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via waitAndWrite()
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class WaitAndWriteActor>
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WaitAndWriteActorState {
															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WaitAndWriteActorState(AFCPage* const& self,void const* const& data,int const& length,int const& offset) 
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self),
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   data(data),
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   length(length),
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   offset(offset)
															#line 1655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("waitAndWrite", reinterpret_cast<unsigned long>(this));

	}
	~WaitAndWriteActorState() 
	{
		fdb_probe_actor_destroy("waitAndWrite", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			StrictFuture<Void> __when_expr_0 = self->notReading;
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (static_cast<WaitAndWriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitAndWriteActor*>(this)->actor_wait_state = 1;
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitAndWriteActor, 0, Void >*>(static_cast<WaitAndWriteActor*>(this)));
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~WaitAndWriteActorState();
		static_cast<WaitAndWriteActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		memcpy(static_cast<uint8_t*>(self->data) + offset, data, length);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WaitAndWriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndWriteActorState(); static_cast<WaitAndWriteActor*>(this)->destroy(); return 0; }
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WaitAndWriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndWriteActorState();
		static_cast<WaitAndWriteActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		memcpy(static_cast<uint8_t*>(self->data) + offset, data, length);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WaitAndWriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndWriteActorState(); static_cast<WaitAndWriteActor*>(this)->destroy(); return 0; }
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WaitAndWriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndWriteActorState();
		static_cast<WaitAndWriteActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<WaitAndWriteActor*>(this)->actor_wait_state > 0) static_cast<WaitAndWriteActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndWriteActor*>(this)->ActorCallback< WaitAndWriteActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAndWriteActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAndWrite", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndWrite", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitAndWriteActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAndWrite", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndWrite", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitAndWriteActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndWrite", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndWrite", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AFCPage* self;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	void const* data;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int length;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int offset;
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via waitAndWrite()
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WaitAndWriteActor final : public Actor<Void>, public ActorCallback< WaitAndWriteActor, 0, Void >, public FastAllocated<WaitAndWriteActor>, public WaitAndWriteActorState<WaitAndWriteActor> {
															#line 1800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<WaitAndWriteActor>::operator new;
	using FastAllocated<WaitAndWriteActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitAndWriteActor, 0, Void >;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WaitAndWriteActor(AFCPage* const& self,void const* const& data,int const& length,int const& offset) 
															#line 1811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   WaitAndWriteActorState<WaitAndWriteActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("waitAndWrite", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitAndWrite");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitAndWrite", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitAndWriteActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> waitAndWrite( AFCPage* const& self, void const* const& data, int const& length, int const& offset ) {
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new WaitAndWriteActor(self, data, length, offset));
															#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	Future<Void> readZeroCopy() {
		++zeroCopyRefCount;
		if (valid) {
			++owner->countFileCachePageReadsHit;
			++owner->countCachePageReadsHit;
			return yield();
		}

		++owner->countFileCachePageReadsMissed;
		++owner->countCachePageReadsMissed;

		if (notReading.isReady()) {
			notReading = readThrough(this);
		} else {
			++owner->countFileCachePageReadsMerged;
			++owner->countCachePageReadsMerged;
		}

		return notReading;
	}
	void releaseZeroCopy() {
		--zeroCopyRefCount;
		ASSERT(zeroCopyRefCount >= 0);
	}

	Future<Void> read(void* data, int length, int offset) {
		if (valid) {
			++owner->countFileCachePageReadsHit;
			++owner->countCachePageReadsHit;
			owner->countFileCacheReadBytes += length;
			owner->countCacheReadBytes += length;
			memcpy(data, static_cast<uint8_t const*>(this->data) + offset, length);
			return yield();
		}

		++owner->countFileCachePageReadsMissed;
		++owner->countCachePageReadsMissed;

		if (notReading.isReady()) {
			notReading = readThrough(this);
		} else {
			++owner->countFileCachePageReadsMerged;
			++owner->countCachePageReadsMerged;
		}

		notReading = waitAndRead(this, data, length, offset);

		return notReading;
	}

																#line 1893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via waitAndRead()
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class WaitAndReadActor>
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WaitAndReadActorState {
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WaitAndReadActorState(AFCPage* const& self,void* const& data,int const& length,int const& offset) 
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self),
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   data(data),
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   length(length),
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   offset(offset)
															#line 1912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("waitAndRead", reinterpret_cast<unsigned long>(this));

	}
	~WaitAndReadActorState() 
	{
		fdb_probe_actor_destroy("waitAndRead", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			StrictFuture<Void> __when_expr_0 = self->notReading;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (static_cast<WaitAndReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitAndReadActor*>(this)->actor_wait_state = 1;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitAndReadActor, 0, Void >*>(static_cast<WaitAndReadActor*>(this)));
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~WaitAndReadActorState();
		static_cast<WaitAndReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		memcpy(data, static_cast<uint8_t const*>(self->data) + offset, length);
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WaitAndReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndReadActorState(); static_cast<WaitAndReadActor*>(this)->destroy(); return 0; }
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WaitAndReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndReadActorState();
		static_cast<WaitAndReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		memcpy(data, static_cast<uint8_t const*>(self->data) + offset, length);
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WaitAndReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndReadActorState(); static_cast<WaitAndReadActor*>(this)->destroy(); return 0; }
															#line 1973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WaitAndReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndReadActorState();
		static_cast<WaitAndReadActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<WaitAndReadActor*>(this)->actor_wait_state > 0) static_cast<WaitAndReadActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndReadActor*>(this)->ActorCallback< WaitAndReadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAndReadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAndRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitAndReadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAndRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitAndReadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndRead", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AFCPage* self;
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	void* data;
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int length;
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	int offset;
															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via waitAndRead()
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WaitAndReadActor final : public Actor<Void>, public ActorCallback< WaitAndReadActor, 0, Void >, public FastAllocated<WaitAndReadActor>, public WaitAndReadActorState<WaitAndReadActor> {
															#line 2057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<WaitAndReadActor>::operator new;
	using FastAllocated<WaitAndReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitAndReadActor, 0, Void >;
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WaitAndReadActor(AFCPage* const& self,void* const& data,int const& length,int const& offset) 
															#line 2068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   WaitAndReadActorState<WaitAndReadActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("waitAndRead", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitAndRead");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitAndRead", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitAndReadActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> waitAndRead( AFCPage* const& self, void* const& data, int const& length, int const& offset ) {
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new WaitAndReadActor(self, data, length, offset));
															#line 2095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

																#line 2100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via readThrough()
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class ReadThroughActor>
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class ReadThroughActorState {
															#line 2106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	ReadThroughActorState(AFCPage* const& self) 
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self)
															#line 2113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("readThrough", reinterpret_cast<unsigned long>(this));

	}
	~ReadThroughActorState() 
	{
		fdb_probe_actor_destroy("readThrough", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			ASSERT(!self->valid);
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			dst = self->data;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (self->pageOffset < self->owner->prevLength)
															#line 2132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			{
				try {
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					StrictFuture<int> __when_expr_0 = self->owner->uncached->read(dst, self->pageCache->pageSize, self->pageOffset);
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					if (static_cast<ReadThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ReadThroughActor*>(this)->actor_wait_state = 1;
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadThroughActor, 0, int >*>(static_cast<ReadThroughActor*>(this)));
															#line 2144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
					loopDepth = 0;
				}
				catch (Error& error) {
					loopDepth = a_body1Catch2(error, loopDepth);
				} catch (...) {
					loopDepth = a_body1Catch2(unknown_error(), loopDepth);
				}
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
		this->~ReadThroughActorState();
		static_cast<ReadThroughActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (dst == self->data)
															#line 2178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			self->valid = true;
															#line 2182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<ReadThroughActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadThroughActorState(); static_cast<ReadThroughActor*>(this)->destroy(); return 0; }
															#line 2186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<ReadThroughActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadThroughActorState();
		static_cast<ReadThroughActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			self->zeroCopyRefCount = 0;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			TraceEvent("ReadThroughFailed").error(e);
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 2209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(int const& _,int loopDepth) 
	{
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (_ != self->pageCache->pageSize)
															#line 2223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			TraceEvent("ReadThroughShortRead") .detail("ReadAmount", _) .detail("PageSize", self->pageCache->pageSize) .detail("PageOffset", self->pageOffset);
															#line 2227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(int && _,int loopDepth) 
	{
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (_ != self->pageCache->pageSize)
															#line 2237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			TraceEvent("ReadThroughShortRead") .detail("ReadAmount", _) .detail("PageSize", self->pageCache->pageSize) .detail("PageOffset", self->pageOffset);
															#line 2241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1when1(int const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadThroughActor*>(this)->actor_wait_state > 0) static_cast<ReadThroughActor*>(this)->actor_wait_state = 0;
		static_cast<ReadThroughActor*>(this)->ActorCallback< ReadThroughActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< ReadThroughActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadThroughActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadThroughActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AFCPage* self;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	void* dst;
															#line 2327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via readThrough()
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class ReadThroughActor final : public Actor<Void>, public ActorCallback< ReadThroughActor, 0, int >, public FastAllocated<ReadThroughActor>, public ReadThroughActorState<ReadThroughActor> {
															#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<ReadThroughActor>::operator new;
	using FastAllocated<ReadThroughActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadThroughActor, 0, int >;
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	ReadThroughActor(AFCPage* const& self) 
															#line 2343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   ReadThroughActorState<ReadThroughActor>(self)
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readThrough");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadThroughActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> readThrough( AFCPage* const& self ) {
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new ReadThroughActor(self));
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

																#line 2375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via writeThrough()
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class WriteThroughActor>
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WriteThroughActorState {
															#line 2381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WriteThroughActorState(AFCPage* const& self,Promise<Void> const& writing) 
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self),
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		   writing(writing)
															#line 2390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("writeThrough", reinterpret_cast<unsigned long>(this));

	}
	~WriteThroughActorState() 
	{
		fdb_probe_actor_destroy("writeThrough", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				dirty = self->dirty;
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				++self->writeThroughCount;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				self->updateFlushableIndex();
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				StrictFuture<Void> __when_expr_0 = self->notReading && self->notFlushing;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (static_cast<WriteThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WriteThroughActor*>(this)->actor_wait_state = 1;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteThroughActor, 0, Void >*>(static_cast<WriteThroughActor*>(this)));
															#line 2419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~WriteThroughActorState();
		static_cast<WriteThroughActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		--self->writeThroughCount;
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		self->updateFlushableIndex();
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		writing.send(Void());
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		self->pageCache->try_evict();
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<WriteThroughActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteThroughActorState(); static_cast<WriteThroughActor*>(this)->destroy(); return 0; }
															#line 2456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<WriteThroughActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteThroughActorState();
		static_cast<WriteThroughActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			--self->writeThroughCount;
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			self->setDirty();
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			writing.sendError(e);
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 2475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (dirty)
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (self->owner->getRateControl())
															#line 2493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				int allowance = 1;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (FLOW_KNOBS->FLOW_CACHEDFILE_WRITE_IO_SIZE > 0)
															#line 2499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				{
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					allowance = (self->pageCache->pageSize + FLOW_KNOBS->FLOW_CACHEDFILE_WRITE_IO_SIZE - 1) / FLOW_KNOBS->FLOW_CACHEDFILE_WRITE_IO_SIZE;
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					ASSERT(allowance > 0);
															#line 2505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				}
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				StrictFuture<Void> __when_expr_1 = self->owner->getRateControl()->getAllowance(allowance);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (static_cast<WriteThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
				static_cast<WriteThroughActor*>(this)->actor_wait_state = 2;
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteThroughActor, 1, Void >*>(static_cast<WriteThroughActor*>(this)));
															#line 2516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont4(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (dirty)
															#line 2535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (self->owner->getRateControl())
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				int allowance = 1;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (FLOW_KNOBS->FLOW_CACHEDFILE_WRITE_IO_SIZE > 0)
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				{
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					allowance = (self->pageCache->pageSize + FLOW_KNOBS->FLOW_CACHEDFILE_WRITE_IO_SIZE - 1) / FLOW_KNOBS->FLOW_CACHEDFILE_WRITE_IO_SIZE;
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
					ASSERT(allowance > 0);
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				}
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				StrictFuture<Void> __when_expr_1 = self->owner->getRateControl()->getAllowance(allowance);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				if (static_cast<WriteThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
				static_cast<WriteThroughActor*>(this)->actor_wait_state = 2;
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteThroughActor, 1, Void >*>(static_cast<WriteThroughActor*>(this)));
															#line 2562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont4(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

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
		if (static_cast<WriteThroughActor*>(this)->actor_wait_state > 0) static_cast<WriteThroughActor*>(this)->actor_wait_state = 0;
		static_cast<WriteThroughActor*>(this)->ActorCallback< WriteThroughActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteThroughActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteThroughActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteThroughActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(int loopDepth) 
	{
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (self->pageOffset + self->pageCache->pageSize > self->owner->length)
															#line 2650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		{
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			ASSERT(self->pageOffset < self->owner->length);
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			memset(static_cast<uint8_t*>(self->data) + self->owner->length - self->pageOffset, 0, self->pageCache->pageSize - (self->owner->length - self->pageOffset));
															#line 2656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		}
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		auto f = self->owner->uncached->write(self->data, self->pageCache->pageSize, self->pageOffset);
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		StrictFuture<Void> __when_expr_2 = f;
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (static_cast<WriteThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteThroughActor*>(this)->actor_wait_state = 3;
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteThroughActor, 2, Void >*>(static_cast<WriteThroughActor*>(this)));
															#line 2669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WriteThroughActor*>(this)->actor_wait_state > 0) static_cast<WriteThroughActor*>(this)->actor_wait_state = 0;
		static_cast<WriteThroughActor*>(this)->ActorCallback< WriteThroughActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteThroughActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteThroughActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteThroughActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WriteThroughActor*>(this)->actor_wait_state > 0) static_cast<WriteThroughActor*>(this)->actor_wait_state = 0;
		static_cast<WriteThroughActor*>(this)->ActorCallback< WriteThroughActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteThroughActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteThroughActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteThroughActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont9(int loopDepth) 
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
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AFCPage* self;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Promise<Void> writing;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	bool dirty;
															#line 2843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via writeThrough()
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class WriteThroughActor final : public Actor<Void>, public ActorCallback< WriteThroughActor, 0, Void >, public ActorCallback< WriteThroughActor, 1, Void >, public ActorCallback< WriteThroughActor, 2, Void >, public FastAllocated<WriteThroughActor>, public WriteThroughActorState<WriteThroughActor> {
															#line 2848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<WriteThroughActor>::operator new;
	using FastAllocated<WriteThroughActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteThroughActor, 0, Void >;
friend struct ActorCallback< WriteThroughActor, 1, Void >;
friend struct ActorCallback< WriteThroughActor, 2, Void >;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	WriteThroughActor(AFCPage* const& self,Promise<Void> const& writing) 
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   WriteThroughActorState<WriteThroughActor>(self, writing)
	{
		fdb_probe_actor_enter("writeThrough", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("writeThrough");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("writeThrough", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteThroughActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteThroughActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteThroughActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> writeThrough( AFCPage* const& self, Promise<Void> const& writing ) {
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new WriteThroughActor(self, writing));
															#line 2890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	Future<Void> flush() {
		if (!dirty && notFlushing.isReady())
			return Void();

		ASSERT(valid || !notReading.isReady() || notReading.isError());

		Promise<Void> writing;

		notFlushing = writeThrough(this, writing);

		clearDirty(); // Do this last so that if writeThrough immediately calls try_evict, we can't be evicted before
		              // assigning notFlushing
		return writing.getFuture();
	}

	Future<Void> quiesce() {
		if (dirty)
			flush();

		// If we are flushing, we will be quiescent when all flushes are finished
		// Returning flush() isn't right, because flush can return before notFlushing.isReady()
		if (!notFlushing.isReady()) {
			return notFlushing;
		}

		// else if we are reading, we will be quiescent when the read is finished
		if (!notReading.isReady())
			return notReading;

		return Void();
	}

	Future<Void> truncate() {
		// Allow truncatation during zero copy reads but orphan the previous buffer
		if (zeroCopyRefCount != 0)
			orphan();
		truncated = true;
		return truncate_impl(this);
	}

																#line 2935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
// This generated class is to be used only via truncate_impl()
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
template <class Truncate_implActor1>
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Truncate_implActor1State {
															#line 2941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Truncate_implActor1State(AFCPage* const& self) 
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		 : self(self)
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
	{
		fdb_probe_actor_create("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	~Truncate_implActor1State() 
	{
		fdb_probe_actor_destroy("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			StrictFuture<Void> __when_expr_0 = self->notReading && self->notFlushing && yield();
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			if (static_cast<Truncate_implActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Truncate_implActor1*>(this)->actor_wait_state = 1;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor1, 0, Void >*>(static_cast<Truncate_implActor1*>(this)));
															#line 2970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
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
		this->~Truncate_implActor1State();
		static_cast<Truncate_implActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		delete self;
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Truncate_implActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActor1State(); static_cast<Truncate_implActor1*>(this)->destroy(); return 0; }
															#line 2995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Truncate_implActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActor1State();
		static_cast<Truncate_implActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		delete self;
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
		if (!static_cast<Truncate_implActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActor1State(); static_cast<Truncate_implActor1*>(this)->destroy(); return 0; }
															#line 3009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		new (&static_cast<Truncate_implActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActor1State();
		static_cast<Truncate_implActor1*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Truncate_implActor1*>(this)->actor_wait_state > 0) static_cast<Truncate_implActor1*>(this)->actor_wait_state = 0;
		static_cast<Truncate_implActor1*>(this)->ActorCallback< Truncate_implActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Truncate_implActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Truncate_implActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Truncate_implActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	AFCPage* self;
															#line 3082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
};
// This generated class is to be used only via truncate_impl()
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
class Truncate_implActor1 final : public Actor<Void>, public ActorCallback< Truncate_implActor1, 0, Void >, public FastAllocated<Truncate_implActor1>, public Truncate_implActor1State<Truncate_implActor1> {
															#line 3087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
public:
	using FastAllocated<Truncate_implActor1>::operator new;
	using FastAllocated<Truncate_implActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Truncate_implActor1, 0, Void >;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	Truncate_implActor1(AFCPage* const& self) 
															#line 3098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
		 : Actor<Void>(),
		   Truncate_implActor1State<Truncate_implActor1>(self)
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("truncate_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Truncate_implActor1, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
[[nodiscard]] static Future<Void> truncate_impl( AFCPage* const& self ) {
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"
	return Future<Void>(new Truncate_implActor1(self));
															#line 3125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.g.h"
}

#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileCached.actor.h"

	AFCPage(AsyncFileCached* owner, int64_t offset)
	  : EvictablePage(owner->pageCache), owner(owner), pageOffset(offset), notReading(Void()), notFlushing(Void()),
	    dirty(false), valid(false), truncated(false), writeThroughCount(0), flushableIndex(-1), zeroCopyRefCount(0) {
		pageCache->allocate(this);
	}

	~AFCPage() override {
		clearDirty();
		ASSERT_ABORT(flushableIndex == -1);
	}

	void setDirty() {
		dirty = true;
		updateFlushableIndex();
	}

	void clearDirty() {
		dirty = false;
		updateFlushableIndex();
	}

	void updateFlushableIndex() {
		bool flushable = dirty || writeThroughCount;
		if (flushable == (flushableIndex != -1))
			return;

		if (flushable) {
			flushableIndex = owner->flushable.size();
			owner->flushable.push_back(this);
		} else {
			ASSERT(owner->flushable[flushableIndex] == this);
			owner->flushable[flushableIndex] = owner->flushable.back();
			owner->flushable[flushableIndex]->flushableIndex = flushableIndex;
			owner->flushable.pop_back();
			flushableIndex = -1;
		}
	}

	AsyncFileCached* owner;
	int64_t pageOffset;

	Future<Void> notReading; // .isReady when a readThrough (or waitAndWrite) is not in progress
	Future<Void> notFlushing; // .isReady when a writeThrough is not in progress

	bool dirty; // write has been called more recently than flush
	bool valid; // data contains the file contents
	bool truncated; // true if this page has been truncated
	int writeThroughCount; // number of writeThrough actors that are in progress (potentially writing or waiting to
	                       // write)
	int flushableIndex; // index in owner->flushable[]
	int zeroCopyRefCount; // references held by "zero-copy" reads
};

#include "flow/unactorcompiler.h"
#endif
