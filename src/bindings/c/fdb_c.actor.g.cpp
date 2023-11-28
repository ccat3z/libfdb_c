#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
#include <cstdint>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <foundationdb/fdb_c_types.h>
#include <flow/ThreadHelper.actor.h>
#include <flow/network.h>
#include <flow/Arena.h>
#include <flow/flow.h>
#include <flow/actorcompiler.h>

namespace foundationdb::extension {
class ThreadUnsafeRWLock : NonCopyable {
public:
	ThreadUnsafeRWLock() : read(true), count(0) {}

	Future<Void> lock_shared();
	Future<Void> lock();
	void unlock();
	bool isFree() { return read && count == 0; };

	struct Acquire {
		bool read;
		Promise<Void> promise;
	};

private:
	bool read : 1;
	uint32_t count : 31;
	std::queue<Acquire> acquire_queue;
};

Future<Void> ThreadUnsafeRWLock::lock_shared() {
	Promise<Void> promise;
	auto future = promise.getFuture();

	if (acquire_queue.empty() && read) {
		++count; // FIXME: count may overflow
		promise.send(Void());
	} else {
		acquire_queue.emplace(Acquire{ true, std::move(promise) });
	}

	return future;
}

Future<Void> ThreadUnsafeRWLock::lock() {
	Promise<Void> promise;
	auto future = promise.getFuture();

	if (acquire_queue.empty() && read && count == 0) {
		read = false;
		promise.send(Void());
	} else {
		acquire_queue.emplace(Acquire{ false, std::move(promise) });
	}

	return future;
}

void ThreadUnsafeRWLock::unlock() {
	if (read) {
		ASSERT(count != 0);

		--count;
		if (count == 0 && !acquire_queue.empty()) {
			auto& r = acquire_queue.front();
			ASSERT(!r.read);

			read = false;
			r.promise.send(Void());
			acquire_queue.pop();
		}
	} else {
		read = true;

		// No pending acquire
		if (acquire_queue.empty())
			return;

		// Pop one write lock
		{
			auto& r = acquire_queue.front();
			if (!r.read) {
				read = false;
				r.promise.send(Void());
				acquire_queue.pop();
				return;
			}
		}

		// OR pop all read lock
		while (!acquire_queue.empty()) {
			auto& r = acquire_queue.front();
			if (!r.read)
				break;

			++count;
			r.promise.send(Void());
			acquire_queue.pop();
		}
	}
}

struct KeysHolder {
	bool locked = false;
	std::unordered_set<std::string> r_keys;
	std::unordered_set<std::string> w_keys;
};

static std::unordered_map<std::string, ThreadUnsafeRWLock> locks;
															#line 115 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
// This generated class is to be used only via acquire_locks()
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
template <class Acquire_locksActor>
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
class Acquire_locksActorState {
															#line 121 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
public:
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
	Acquire_locksActorState(KeysHolder* const& holder) 
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		 : holder(holder)
															#line 128 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
	{

	}
	~Acquire_locksActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
			if (holder->locked)
															#line 141 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
			{
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
				if (!static_cast<Acquire_locksActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Acquire_locksActorState(); static_cast<Acquire_locksActor*>(this)->destroy(); return 0; }
															#line 145 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
				new (&static_cast<Acquire_locksActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~Acquire_locksActorState();
				static_cast<Acquire_locksActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
			futures = std::vector<Future<Void>>();
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
			for( auto& key : holder->r_keys ) {
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
				futures.emplace_back(locks[key].lock_shared());
															#line 157 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
			}
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
			for( auto& key : holder->w_keys ) {
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
				futures.emplace_back(locks[key].lock());
															#line 163 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
			}
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
			i = 0;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
			;
															#line 169 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
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
		this->~Acquire_locksActorState();
		static_cast<Acquire_locksActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		holder->locked = true;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		if (!static_cast<Acquire_locksActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Acquire_locksActorState(); static_cast<Acquire_locksActor*>(this)->destroy(); return 0; }
															#line 194 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
		new (&static_cast<Acquire_locksActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Acquire_locksActorState();
		static_cast<Acquire_locksActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		if (!(i < futures.size()))
															#line 213 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		StrictFuture<Void> __when_expr_0 = futures[i];
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		if (static_cast<Acquire_locksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 221 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Acquire_locksActor*>(this)->actor_wait_state = 1;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Acquire_locksActor, 0, Void >*>(static_cast<Acquire_locksActor*>(this)));
															#line 226 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
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
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		++i;
															#line 248 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
		++i;
															#line 257 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

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
		if (static_cast<Acquire_locksActor*>(this)->actor_wait_state > 0) static_cast<Acquire_locksActor*>(this)->actor_wait_state = 0;
		static_cast<Acquire_locksActor*>(this)->ActorCallback< Acquire_locksActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Acquire_locksActor, 0, Void >*,Void const& value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< Acquire_locksActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< Acquire_locksActor, 0, Void >*,Error err) 
	{
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
	KeysHolder* holder;
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
	std::vector<Future<Void>> futures;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
	int i;
															#line 325 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
};
// This generated class is to be used only via acquire_locks()
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
class Acquire_locksActor final : public Actor<Void>, public ActorCallback< Acquire_locksActor, 0, Void >, public FastAllocated<Acquire_locksActor>, public Acquire_locksActorState<Acquire_locksActor> {
															#line 330 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
public:
	using FastAllocated<Acquire_locksActor>::operator new;
	using FastAllocated<Acquire_locksActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Acquire_locksActor, 0, Void >;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
	Acquire_locksActor(KeysHolder* const& holder) 
															#line 341 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
		 : Actor<Void>(),
		   Acquire_locksActorState<Acquire_locksActor>(holder)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("acquire_locks");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Acquire_locksActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
[[nodiscard]] Future<Void> acquire_locks( KeysHolder* const& holder ) {
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"
	return Future<Void>(new Acquire_locksActor(holder));
															#line 366 "/usr/src/libfdb_c/bindings/c/fdb_c.actor.g.cpp"
}

#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/extractor/workdir/patches/fdb_c.actor.cpp"

void release_locks(KeysHolder* holder) {
	if (!holder->locked)
		return;

	for (auto& key : holder->r_keys) {
		// std::cerr << "Unlock shared lock " << key << std::endl;

		auto it = locks.find(key);
		ASSERT(it != locks.end());

		it->second.unlock();
		if (it->second.isFree())
			locks.erase(it);
	}

	for (auto& key : holder->w_keys) {
		// std::cerr << "Unlock exclusive lock " << key << std::endl;

		auto it = locks.find(key);
		ASSERT(it != locks.end());

		it->second.unlock();
		if (it->second.isFree())
			locks.erase(it);
	}
}

extern "C" {
DLLEXPORT void* fdb_rwlocks_create() {
	return new KeysHolder();
}

DLLEXPORT void fdb_rwlocks_shared(void* holder, const char* key) {
	ASSERT(holder);
	auto h = (KeysHolder*)holder;

	auto w_key = h->w_keys.find(key);
	if (w_key != h->w_keys.end())
		return;

	h->r_keys.emplace(key);
}

DLLEXPORT void fdb_rwlocks_exclusive(void* holder, const char* key) {
	ASSERT(holder);
	auto h = (KeysHolder*)holder;

	auto r_key = h->r_keys.find(key);
	if (r_key != h->r_keys.end())
		h->r_keys.erase(key);

	h->w_keys.emplace(key);
}

DLLEXPORT FDBFuture* fdb_rwlocks_lock(void* holder) {
	ASSERT(holder);
	auto h = (KeysHolder*)holder;
	return (FDBFuture*)onMainThread([=]() { return acquire_locks(h); }).extractPtr();
}

DLLEXPORT void fdb_rwlocks_free(void* holder) {
	ASSERT(holder);
	auto h = (KeysHolder*)holder;
	onMainThreadVoid([=]() {
		release_locks(h);
		delete h;
	});
}

DLLEXPORT FDBFuture* fdb_delay(double seconds) {
	return (FDBFuture*)onMainThread([=]() { return g_network->delay(seconds, TaskPriority::DefaultDelay); })
	    .extractPtr();
}
}
} // namespace foundationdb::extension