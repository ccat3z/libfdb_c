#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
/*
 * IAsyncFile.actor.cpp
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

#include "fdbrpc/IAsyncFile.h"
#include "flow/Error.h"
#include "flow/Knobs.h"
#include "flow/Platform.h"
#include "flow/UnitTest.h"
#include <iostream>
#include "flow/actorcompiler.h" // has to be last include

IAsyncFile::~IAsyncFile() = default;

const static unsigned int ONE_MEGABYTE = 1 << 20;
const static unsigned int FOUR_KILOBYTES = 4 << 10;

															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
namespace {
// This generated class is to be used only via zeroRangeHelper()
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
template <class ZeroRangeHelperActor>
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class ZeroRangeHelperActorState {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	ZeroRangeHelperActorState(Reference<IAsyncFile> const& f,int64_t const& offset,int64_t const& length,int const& fixedbyte) 
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		 : f(f),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   offset(offset),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   length(length),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   fixedbyte(fixedbyte),
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   pos(offset),
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   zeros(aligned_alloc(ONE_MEGABYTE, ONE_MEGABYTE))
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
	{
		fdb_probe_actor_create("zeroRangeHelper", reinterpret_cast<unsigned long>(this));

	}
	~ZeroRangeHelperActorState() 
	{
		fdb_probe_actor_destroy("zeroRangeHelper", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			memset(zeros, fixedbyte, ONE_MEGABYTE);
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
		this->~ZeroRangeHelperActorState();
		static_cast<ZeroRangeHelperActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		aligned_free(zeros);
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<ZeroRangeHelperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ZeroRangeHelperActorState(); static_cast<ZeroRangeHelperActor*>(this)->destroy(); return 0; }
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<ZeroRangeHelperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ZeroRangeHelperActorState();
		static_cast<ZeroRangeHelperActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!(pos < offset + length))
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		len = std::min<int64_t>(ONE_MEGABYTE, offset + length - pos);
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_0 = f->write(zeros, len, pos);
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state = 1;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ZeroRangeHelperActor, 0, Void >*>(static_cast<ZeroRangeHelperActor*>(this)));
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		pos += len;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_1 = yield();
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state = 2;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ZeroRangeHelperActor, 1, Void >*>(static_cast<ZeroRangeHelperActor*>(this)));
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		pos += len;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_1 = yield();
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state = 2;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ZeroRangeHelperActor, 1, Void >*>(static_cast<ZeroRangeHelperActor*>(this)));
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state > 0) static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state = 0;
		static_cast<ZeroRangeHelperActor*>(this)->ActorCallback< ZeroRangeHelperActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ZeroRangeHelperActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ZeroRangeHelperActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ZeroRangeHelperActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state > 0) static_cast<ZeroRangeHelperActor*>(this)->actor_wait_state = 0;
		static_cast<ZeroRangeHelperActor*>(this)->ActorCallback< ZeroRangeHelperActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ZeroRangeHelperActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ZeroRangeHelperActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ZeroRangeHelperActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	Reference<IAsyncFile> f;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t offset;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t length;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int fixedbyte;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t pos;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	void* zeros;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int len;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
};
// This generated class is to be used only via zeroRangeHelper()
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class ZeroRangeHelperActor final : public Actor<Void>, public ActorCallback< ZeroRangeHelperActor, 0, Void >, public ActorCallback< ZeroRangeHelperActor, 1, Void >, public FastAllocated<ZeroRangeHelperActor>, public ZeroRangeHelperActorState<ZeroRangeHelperActor> {
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
	using FastAllocated<ZeroRangeHelperActor>::operator new;
	using FastAllocated<ZeroRangeHelperActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ZeroRangeHelperActor, 0, Void >;
friend struct ActorCallback< ZeroRangeHelperActor, 1, Void >;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	ZeroRangeHelperActor(Reference<IAsyncFile> const& f,int64_t const& offset,int64_t const& length,int const& fixedbyte) 
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		 : Actor<Void>(),
		   ZeroRangeHelperActorState<ZeroRangeHelperActor>(f, offset, length, fixedbyte)
	{
		fdb_probe_actor_enter("zeroRangeHelper", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("zeroRangeHelper");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("zeroRangeHelper", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ZeroRangeHelperActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ZeroRangeHelperActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
[[nodiscard]] static Future<Void> zeroRangeHelper( Reference<IAsyncFile> const& f, int64_t const& offset, int64_t const& length, int const& fixedbyte ) {
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	return Future<Void>(new ZeroRangeHelperActor(f, offset, length, fixedbyte));
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
}

#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"

Future<Void> IAsyncFile::zeroRange(int64_t offset, int64_t length) {
	return uncancellable(zeroRangeHelper(Reference<IAsyncFile>::addRef(this), offset, length, 0));
}

															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase54()
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
template <class FlowTestCase54Actor>
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase54ActorState {
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase54ActorState(UnitTestParameters const& params) 
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		 : params(params),
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   filename("/tmp/__ZEROJUNK__")
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase54", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase54ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase54", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_CREATE | IAsyncFile::OPEN_READWRITE, 0);
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 1;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
		this->~FlowTestCase54ActorState();
		static_cast<FlowTestCase54Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->sync();
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 2;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 1, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f = __f;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->zeroRange(0, ONE_MEGABYTE);
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 3;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 2, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->zeroRange(0, ONE_MEGABYTE);
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 3;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 2, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int64_t> __when_expr_3 = f->size();
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 4;
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 3, int64_t >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int64_t> __when_expr_3 = f->size();
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 4;
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 3, int64_t >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(int64_t const& size,int loopDepth) 
	{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(ONE_MEGABYTE == size);
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = zeroRangeHelper(f, 0, ONE_MEGABYTE, 0xff);
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 5;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 4, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(int64_t && size,int loopDepth) 
	{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(ONE_MEGABYTE == size);
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = zeroRangeHelper(f, 0, ONE_MEGABYTE, 0xff);
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 5;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 4, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(int64_t const& size,int loopDepth) 
	{
		loopDepth = a_body1cont4(size, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(int64_t && size,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(size), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 3, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 3, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 3, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 3, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_5 = f->zeroRange(0, ONE_MEGABYTE);
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont5when1(__when_expr_5.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 6;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 5, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_5 = f->zeroRange(0, ONE_MEGABYTE);
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont5when1(__when_expr_5.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 6;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 5, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		page = (uint8_t*)malloc(FOUR_KILOBYTES);
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int> __when_expr_6 = f->read(page, FOUR_KILOBYTES, 0);
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont6when1(__when_expr_6.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 7;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 6, int >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		page = (uint8_t*)malloc(FOUR_KILOBYTES);
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int> __when_expr_6 = f->read(page, FOUR_KILOBYTES, 0);
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont6when1(__when_expr_6.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 7;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 6, int >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont7(int const& n,int loopDepth) 
	{
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(n == FOUR_KILOBYTES);
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for(int i = 0;i < FOUR_KILOBYTES;i++) {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			ASSERT(page[i] == 0);
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		free(page);
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_7 = IAsyncFileSystem::filesystem()->deleteFile(filename, true);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont7when1(__when_expr_7.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 8;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 7, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 1043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7(int && n,int loopDepth) 
	{
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(n == FOUR_KILOBYTES);
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for(int i = 0;i < FOUR_KILOBYTES;i++) {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			ASSERT(page[i] == 0);
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		free(page);
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_7 = IAsyncFileSystem::filesystem()->deleteFile(filename, true);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont7when1(__when_expr_7.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 8;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 7, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6when1(int const& n,int loopDepth) 
	{
		loopDepth = a_body1cont7(n, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(int && n,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(n), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 6, int >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 6, int >*,int const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 6, int >*,int && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 6, int >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase54Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase54ActorState(); static_cast<FlowTestCase54Actor*>(this)->destroy(); return 0; }
															#line 1143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase54Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase54ActorState();
		static_cast<FlowTestCase54Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase54Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase54ActorState(); static_cast<FlowTestCase54Actor*>(this)->destroy(); return 0; }
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase54Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase54ActorState();
		static_cast<FlowTestCase54Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 7);

	}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	UnitTestParameters params;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::string filename;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	Reference<IAsyncFile> f;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	uint8_t* page;
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase54()
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase54Actor final : public Actor<Void>, public ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >, public ActorCallback< FlowTestCase54Actor, 1, Void >, public ActorCallback< FlowTestCase54Actor, 2, Void >, public ActorCallback< FlowTestCase54Actor, 3, int64_t >, public ActorCallback< FlowTestCase54Actor, 4, Void >, public ActorCallback< FlowTestCase54Actor, 5, Void >, public ActorCallback< FlowTestCase54Actor, 6, int >, public ActorCallback< FlowTestCase54Actor, 7, Void >, public FastAllocated<FlowTestCase54Actor>, public FlowTestCase54ActorState<FlowTestCase54Actor> {
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase54Actor>::operator new;
	using FastAllocated<FlowTestCase54Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< FlowTestCase54Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase54Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase54Actor, 3, int64_t >;
friend struct ActorCallback< FlowTestCase54Actor, 4, Void >;
friend struct ActorCallback< FlowTestCase54Actor, 5, Void >;
friend struct ActorCallback< FlowTestCase54Actor, 6, int >;
friend struct ActorCallback< FlowTestCase54Actor, 7, Void >;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase54Actor(UnitTestParameters const& params) 
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase54ActorState<FlowTestCase54Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase54");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 3, int64_t >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 6, int >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 7, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
static Future<Void> flowTestCase54( UnitTestParameters const& params ) {
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	return Future<Void>(new FlowTestCase54Actor(params));
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase54, "/fileio/zero")

#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"

															#line 1298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
namespace {
// This generated class is to be used only via incrementalDeleteHelper()
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
template <class IncrementalDeleteHelperActor>
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class IncrementalDeleteHelperActorState {
															#line 1305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	IncrementalDeleteHelperActorState(std::string const& filename,bool const& mustBeDurable,int64_t const& truncateAmt,double const& interval) 
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		 : filename(filename),
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   mustBeDurable(mustBeDurable),
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   truncateAmt(truncateAmt),
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   interval(interval),
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   file(),
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   remainingFileSize(),
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   exists(fileExists(filename))
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
	{
		fdb_probe_actor_create("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this));

	}
	~IncrementalDeleteHelperActorState() 
	{
		fdb_probe_actor_destroy("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (exists)
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_READWRITE | IAsyncFile::OPEN_UNCACHED | IAsyncFile::OPEN_UNBUFFERED, 0);
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
				if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 1;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
		this->~IncrementalDeleteHelperActorState();
		static_cast<IncrementalDeleteHelperActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = IAsyncFileSystem::filesystem()->deleteFile(filename, mustBeDurable);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 3;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 2, Void >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Reference<IAsyncFile> const& f,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		file = f;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int64_t> __when_expr_1 = file->size();
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 2;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Reference<IAsyncFile> && f,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		file = f;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int64_t> __when_expr_1 = file->size();
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 2;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& f,int loopDepth) 
	{
		loopDepth = a_body1cont2(f, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && f,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(f), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state > 0) static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalDeleteHelperActor*>(this)->ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int64_t const& fileSize,int loopDepth) 
	{
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		remainingFileSize = fileSize;
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(int64_t && fileSize,int loopDepth) 
	{
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		remainingFileSize = fileSize;
															#line 1502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int64_t const& fileSize,int loopDepth) 
	{
		loopDepth = a_body1cont3(fileSize, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int64_t && fileSize,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(fileSize), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state > 0) static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalDeleteHelperActor*>(this)->ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (exists)
															#line 1574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			;
															#line 1578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			loopDepth = a_body1cont5loopHead1(loopDepth);
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (exists)
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			;
															#line 1596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			loopDepth = a_body1cont5loopHead1(loopDepth);
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state > 0) static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalDeleteHelperActor*>(this)->ActorCallback< IncrementalDeleteHelperActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< IncrementalDeleteHelperActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<IncrementalDeleteHelperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalDeleteHelperActorState(); static_cast<IncrementalDeleteHelperActor*>(this)->destroy(); return 0; }
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<IncrementalDeleteHelperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncrementalDeleteHelperActorState();
		static_cast<IncrementalDeleteHelperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont5loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont5loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5loopBody1(int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!(remainingFileSize > 0))
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		{
			return a_body1cont5break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_3 = file->truncate(remainingFileSize);
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont5loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 4;
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 3, Void >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5break1(int loopDepth) 
	{
		try {
			return a_body1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = file->sync();
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont5loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 5;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 4, Void >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = file->sync();
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont5loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 5;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 4, Void >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state > 0) static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalDeleteHelperActor*>(this)->ActorCallback< IncrementalDeleteHelperActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< IncrementalDeleteHelperActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont5loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_5 = delay(interval);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont5loopBody1cont3when1(__when_expr_5.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 6;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 5, Void >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_5 = delay(interval);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont5loopBody1cont3when1(__when_expr_5.get(), loopDepth); };
		static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 6;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< IncrementalDeleteHelperActor, 5, Void >*>(static_cast<IncrementalDeleteHelperActor*>(this)));
															#line 1851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state > 0) static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalDeleteHelperActor*>(this)->ActorCallback< IncrementalDeleteHelperActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< IncrementalDeleteHelperActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont5loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		remainingFileSize -= truncateAmt;
															#line 1923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont5loopHead1(0);

		return loopDepth;
	}
	int a_body1cont5loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		remainingFileSize -= truncateAmt;
															#line 1932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont5loopHead1(0);

		return loopDepth;
	}
	int a_body1cont5loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state > 0) static_cast<IncrementalDeleteHelperActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalDeleteHelperActor*>(this)->ActorCallback< IncrementalDeleteHelperActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont5loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< IncrementalDeleteHelperActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont5loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< IncrementalDeleteHelperActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::string filename;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	bool mustBeDurable;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t truncateAmt;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	double interval;
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	Reference<IAsyncFile> file;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t remainingFileSize;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	bool exists;
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
};
// This generated class is to be used only via incrementalDeleteHelper()
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class IncrementalDeleteHelperActor final : public Actor<Void>, public ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >, public ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >, public ActorCallback< IncrementalDeleteHelperActor, 2, Void >, public ActorCallback< IncrementalDeleteHelperActor, 3, Void >, public ActorCallback< IncrementalDeleteHelperActor, 4, Void >, public ActorCallback< IncrementalDeleteHelperActor, 5, Void >, public FastAllocated<IncrementalDeleteHelperActor>, public IncrementalDeleteHelperActorState<IncrementalDeleteHelperActor> {
															#line 2019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
	using FastAllocated<IncrementalDeleteHelperActor>::operator new;
	using FastAllocated<IncrementalDeleteHelperActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >;
friend struct ActorCallback< IncrementalDeleteHelperActor, 2, Void >;
friend struct ActorCallback< IncrementalDeleteHelperActor, 3, Void >;
friend struct ActorCallback< IncrementalDeleteHelperActor, 4, Void >;
friend struct ActorCallback< IncrementalDeleteHelperActor, 5, Void >;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	IncrementalDeleteHelperActor(std::string const& filename,bool const& mustBeDurable,int64_t const& truncateAmt,double const& interval) 
															#line 2035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		 : Actor<Void>(),
		   IncrementalDeleteHelperActorState<IncrementalDeleteHelperActor>(filename, mustBeDurable, truncateAmt, interval)
	{
		fdb_probe_actor_enter("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("incrementalDeleteHelper");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("incrementalDeleteHelper", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IncrementalDeleteHelperActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IncrementalDeleteHelperActor, 1, int64_t >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< IncrementalDeleteHelperActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< IncrementalDeleteHelperActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< IncrementalDeleteHelperActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< IncrementalDeleteHelperActor, 5, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
[[nodiscard]] static Future<Void> incrementalDeleteHelper( std::string const& filename, bool const& mustBeDurable, int64_t const& truncateAmt, double const& interval ) {
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	return Future<Void>(new IncrementalDeleteHelperActor(filename, mustBeDurable, truncateAmt, interval));
															#line 2068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
}

#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"

Future<Void> IAsyncFileSystem::incrementalDeleteFile(const std::string& filename, bool mustBeDurable) {
	return uncancellable(incrementalDeleteHelper(filename,
	                                             mustBeDurable,
	                                             FLOW_KNOBS->INCREMENTAL_DELETE_TRUNCATE_AMOUNT,
	                                             FLOW_KNOBS->INCREMENTAL_DELETE_INTERVAL));
}

															#line 2080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase119()
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
template <class FlowTestCase119Actor>
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase119ActorState {
															#line 2087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase119ActorState(UnitTestParameters const& params) 
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		 : params(params),
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   fileSize(5e9),
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   filename("/tmp/__JUNK__")
															#line 2098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase119", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase119ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase119", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_CREATE | IAsyncFile::OPEN_READWRITE, 0);
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 1;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >*>(static_cast<FlowTestCase119Actor*>(this)));
															#line 2120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
		this->~FlowTestCase119ActorState();
		static_cast<FlowTestCase119Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->sync();
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 2;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase119Actor, 1, Void >*>(static_cast<FlowTestCase119Actor*>(this)));
															#line 2150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f = __f;
															#line 2159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase119Actor*>(this)->ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->truncate(fileSize);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 3;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase119Actor, 2, Void >*>(static_cast<FlowTestCase119Actor*>(this)));
															#line 2233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->truncate(fileSize);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 3;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase119Actor, 2, Void >*>(static_cast<FlowTestCase119Actor*>(this)));
															#line 2249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase119Actor*>(this)->ActorCallback< FlowTestCase119Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase119Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_3 = IAsyncFileSystem::filesystem()->incrementalDeleteFile(filename, true);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 4;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase119Actor, 3, Void >*>(static_cast<FlowTestCase119Actor*>(this)));
															#line 2330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_3 = IAsyncFileSystem::filesystem()->incrementalDeleteFile(filename, true);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 4;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase119Actor, 3, Void >*>(static_cast<FlowTestCase119Actor*>(this)));
															#line 2348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase119Actor*>(this)->ActorCallback< FlowTestCase119Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase119Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase119Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase119ActorState(); static_cast<FlowTestCase119Actor*>(this)->destroy(); return 0; }
															#line 2420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase119Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase119ActorState();
		static_cast<FlowTestCase119Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase119Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase119ActorState(); static_cast<FlowTestCase119Actor*>(this)->destroy(); return 0; }
															#line 2432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase119Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase119ActorState();
		static_cast<FlowTestCase119Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase119Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase119Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase119Actor*>(this)->ActorCallback< FlowTestCase119Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase119Actor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase119Actor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	UnitTestParameters params;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t fileSize;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::string filename;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	Reference<IAsyncFile> f;
															#line 2511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase119()
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase119Actor final : public Actor<Void>, public ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >, public ActorCallback< FlowTestCase119Actor, 1, Void >, public ActorCallback< FlowTestCase119Actor, 2, Void >, public ActorCallback< FlowTestCase119Actor, 3, Void >, public FastAllocated<FlowTestCase119Actor>, public FlowTestCase119ActorState<FlowTestCase119Actor> {
															#line 2516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase119Actor>::operator new;
	using FastAllocated<FlowTestCase119Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< FlowTestCase119Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase119Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase119Actor, 3, Void >;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase119Actor(UnitTestParameters const& params) 
															#line 2530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase119ActorState<FlowTestCase119Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase119", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase119");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase119", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase119Actor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase119Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase119Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase119Actor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
static Future<Void> flowTestCase119( UnitTestParameters const& params ) {
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	return Future<Void>(new FlowTestCase119Actor(params));
															#line 2561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase119, "/fileio/incrementalDelete")

#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"

															#line 2567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase133()
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
template <class FlowTestCase133Actor>
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase133ActorState {
															#line 2574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase133ActorState(UnitTestParameters const& params) 
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		 : params(params),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   fileSize(100e6),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   filename("/tmp/__JUNK__." + deterministicRandom()->randomUniqueID().toString()),
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   renamedFile("/tmp/__RENAMED_JUNK__." + deterministicRandom()->randomUniqueID().toString()),
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   data(new char[4096]),
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   readData(new char[4096])
															#line 2591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase133", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase133ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase133", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_CREATE | IAsyncFile::OPEN_READWRITE, 0644);
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 1;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
		this->~FlowTestCase133ActorState();
		static_cast<FlowTestCase133Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->sync();
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 2;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 1, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f = __f;
															#line 2652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->truncate(fileSize);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 3;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 2, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->truncate(fileSize);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 3;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 2, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		memset(data.get(), 0, 4096);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for(int i = 0;i < 16;++i) {
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			data[i] = deterministicRandom()->randomAlphaNumeric();
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_3 = f->write(data.get(), 4096, 0);
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 4;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 3, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		memset(data.get(), 0, 4096);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for(int i = 0;i < 16;++i) {
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			data[i] = deterministicRandom()->randomAlphaNumeric();
															#line 2842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_3 = f->write(data.get(), 4096, 0);
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 4;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 3, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = f->write(data.get(), 4096, fileSize - 4096);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 5;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 4, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = f->write(data.get(), 4096, fileSize - 4096);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 5;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 4, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_5 = f->sync();
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont6when1(__when_expr_5.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 6;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 5, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_5 = f->sync();
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont6when1(__when_expr_5.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 6;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 5, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_6 = IAsyncFileSystem::filesystem()->renameFile(filename, renamedFile);
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont7when1(__when_expr_6.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 7;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 6, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_6 = IAsyncFileSystem::filesystem()->renameFile(filename, renamedFile);
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont7when1(__when_expr_6.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 7;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 6, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_7 = IAsyncFileSystem::filesystem()->open(renamedFile, IAsyncFile::OPEN_READONLY, 0);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont8when1(__when_expr_7.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 8;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_7 = IAsyncFileSystem::filesystem()->open(renamedFile, IAsyncFile::OPEN_READONLY, 0);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont8when1(__when_expr_7.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 8;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont9(Reference<IAsyncFile> const& _f,int loopDepth) 
	{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f = _f;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		bool renamedExists = false;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		auto bName = basename(renamedFile);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		auto files = platform::listFiles("/tmp/");
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for( const auto& file : files ) {
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (file == bName)
															#line 3319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			{
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
				renamedExists = true;
															#line 3323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			}
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			ASSERT(file != filename);
															#line 3327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(renamedExists);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int> __when_expr_8 = f->read(readData.get(), 4096, 0);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1cont9when1(__when_expr_8.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 9;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 8, int >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9(Reference<IAsyncFile> && _f,int loopDepth) 
	{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f = _f;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		bool renamedExists = false;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		auto bName = basename(renamedFile);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		auto files = platform::listFiles("/tmp/");
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for( const auto& file : files ) {
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (file == bName)
															#line 3359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			{
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
				renamedExists = true;
															#line 3363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			}
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			ASSERT(file != filename);
															#line 3367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(renamedExists);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int> __when_expr_8 = f->read(readData.get(), 4096, 0);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1cont9when1(__when_expr_8.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 9;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 8, int >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont8when1(Reference<IAsyncFile> const& _f,int loopDepth) 
	{
		loopDepth = a_body1cont9(_f, loopDepth);

		return loopDepth;
	}
	int a_body1cont8when1(Reference<IAsyncFile> && _f,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_f), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1cont10(int const& length,int loopDepth) 
	{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(length == 4096);
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(memcmp(readData.get(), data.get(), 4096) == 0);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_9 = IAsyncFileSystem::filesystem()->deleteFile(renamedFile, true);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_9.isReady()) { if (__when_expr_9.isError()) return a_body1Catch1(__when_expr_9.getError(), loopDepth); else return a_body1cont10when1(__when_expr_9.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 10;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_9.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 9, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10(int && length,int loopDepth) 
	{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(length == 4096);
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(memcmp(readData.get(), data.get(), 4096) == 0);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_9 = IAsyncFileSystem::filesystem()->deleteFile(renamedFile, true);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_9.isReady()) { if (__when_expr_9.isError()) return a_body1Catch1(__when_expr_9.getError(), loopDepth); else return a_body1cont10when1(__when_expr_9.get(), loopDepth); };
		static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 10;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_9.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase133Actor, 9, Void >*>(static_cast<FlowTestCase133Actor*>(this)));
															#line 3487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9when1(int const& length,int loopDepth) 
	{
		loopDepth = a_body1cont10(length, loopDepth);

		return loopDepth;
	}
	int a_body1cont9when1(int && length,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(length), loopDepth);

		return loopDepth;
	}
	void a_exitChoose9() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 8, int >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 8, int >*,int const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1cont9when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 8, int >*,int && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1cont9when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 8, int >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 8);

	}
	int a_body1cont10cont1(Void const& _,int loopDepth) 
	{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase133Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase133ActorState(); static_cast<FlowTestCase133Actor*>(this)->destroy(); return 0; }
															#line 3559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase133Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase133ActorState();
		static_cast<FlowTestCase133Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont1(Void && _,int loopDepth) 
	{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase133Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase133ActorState(); static_cast<FlowTestCase133Actor*>(this)->destroy(); return 0; }
															#line 3571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase133Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase133ActorState();
		static_cast<FlowTestCase133Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose10() 
	{
		if (static_cast<FlowTestCase133Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase133Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase133Actor*>(this)->ActorCallback< FlowTestCase133Actor, 9, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 9, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 9);

	}
	void a_callback_fire(ActorCallback< FlowTestCase133Actor, 9, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 9);

	}
	void a_callback_error(ActorCallback< FlowTestCase133Actor, 9, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), 9);

	}
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	UnitTestParameters params;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	int64_t fileSize;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::string filename;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::string renamedFile;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::unique_ptr<char[]> data;
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::unique_ptr<char[]> readData;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	Reference<IAsyncFile> f;
															#line 3656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase133()
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase133Actor final : public Actor<Void>, public ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >, public ActorCallback< FlowTestCase133Actor, 1, Void >, public ActorCallback< FlowTestCase133Actor, 2, Void >, public ActorCallback< FlowTestCase133Actor, 3, Void >, public ActorCallback< FlowTestCase133Actor, 4, Void >, public ActorCallback< FlowTestCase133Actor, 5, Void >, public ActorCallback< FlowTestCase133Actor, 6, Void >, public ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >, public ActorCallback< FlowTestCase133Actor, 8, int >, public ActorCallback< FlowTestCase133Actor, 9, Void >, public FastAllocated<FlowTestCase133Actor>, public FlowTestCase133ActorState<FlowTestCase133Actor> {
															#line 3661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase133Actor>::operator new;
	using FastAllocated<FlowTestCase133Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< FlowTestCase133Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase133Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase133Actor, 3, Void >;
friend struct ActorCallback< FlowTestCase133Actor, 4, Void >;
friend struct ActorCallback< FlowTestCase133Actor, 5, Void >;
friend struct ActorCallback< FlowTestCase133Actor, 6, Void >;
friend struct ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >;
friend struct ActorCallback< FlowTestCase133Actor, 8, int >;
friend struct ActorCallback< FlowTestCase133Actor, 9, Void >;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase133Actor(UnitTestParameters const& params) 
															#line 3681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase133ActorState<FlowTestCase133Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase133", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase133");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase133", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 6, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 7, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 9: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 8, int >*)0, actor_cancelled()); break;
		case 10: this->a_callback_error((ActorCallback< FlowTestCase133Actor, 9, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
static Future<Void> flowTestCase133( UnitTestParameters const& params ) {
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	return Future<Void>(new FlowTestCase133Actor(params));
															#line 3718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase133, "/fileio/rename")

#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"

// Truncating to extend size should zero the new data
															#line 3725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase187()
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
template <class FlowTestCase187Actor>
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase187ActorState {
															#line 3732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase187ActorState(UnitTestParameters const& params) 
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		 : params(params),
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		   filename("/tmp/__JUNK__")
															#line 3741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase187", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase187ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase187", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_CREATE | IAsyncFile::OPEN_READWRITE, 0);
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 1;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 3763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
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
		this->~FlowTestCase187ActorState();
		static_cast<FlowTestCase187Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		data = std::array<char, 4096>();
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->sync();
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 2;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 1, Void >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 3795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f = __f;
															#line 3804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase187Actor*>(this)->ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->truncate(4096);
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 3;
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 2, Void >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 3878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->truncate(4096);
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 3;
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 2, Void >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 3894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase187Actor*>(this)->ActorCallback< FlowTestCase187Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase187Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int> __when_expr_3 = f->read(&data[0], 4096, 0);
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 4;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 3, int >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 3973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<int> __when_expr_3 = f->read(&data[0], 4096, 0);
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 4;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 3, int >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 3989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase187Actor*>(this)->ActorCallback< FlowTestCase187Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase187Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(int const& length,int loopDepth) 
	{
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(length == 4096);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for( auto c : data ) {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			ASSERT(c == '\0');
															#line 4065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = IAsyncFileSystem::filesystem()->incrementalDeleteFile(filename, true);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 5;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 4, Void >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 4078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(int && length,int loopDepth) 
	{
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		ASSERT(length == 4096);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		for( auto c : data ) {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
			ASSERT(c == '\0');
															#line 4091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		}
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		f.clear();
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		StrictFuture<Void> __when_expr_4 = IAsyncFileSystem::filesystem()->incrementalDeleteFile(filename, true);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 5;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase187Actor, 4, Void >*>(static_cast<FlowTestCase187Actor*>(this)));
															#line 4104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(int const& length,int loopDepth) 
	{
		loopDepth = a_body1cont4(length, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(int && length,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(length), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase187Actor*>(this)->ActorCallback< FlowTestCase187Actor, 3, int >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 3, int >*,int const& value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 3, int >*,int && value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase187Actor, 3, int >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase187Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase187ActorState(); static_cast<FlowTestCase187Actor*>(this)->destroy(); return 0; }
															#line 4176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase187Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase187ActorState();
		static_cast<FlowTestCase187Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
		if (!static_cast<FlowTestCase187Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase187ActorState(); static_cast<FlowTestCase187Actor*>(this)->destroy(); return 0; }
															#line 4188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		new (&static_cast<FlowTestCase187Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase187ActorState();
		static_cast<FlowTestCase187Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<FlowTestCase187Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase187Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase187Actor*>(this)->ActorCallback< FlowTestCase187Actor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< FlowTestCase187Actor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< FlowTestCase187Actor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	UnitTestParameters params;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::string filename;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	Reference<IAsyncFile> f;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	std::array<char, 4096> data;
															#line 4267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase187()
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
class FlowTestCase187Actor final : public Actor<Void>, public ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >, public ActorCallback< FlowTestCase187Actor, 1, Void >, public ActorCallback< FlowTestCase187Actor, 2, Void >, public ActorCallback< FlowTestCase187Actor, 3, int >, public ActorCallback< FlowTestCase187Actor, 4, Void >, public FastAllocated<FlowTestCase187Actor>, public FlowTestCase187ActorState<FlowTestCase187Actor> {
															#line 4272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase187Actor>::operator new;
	using FastAllocated<FlowTestCase187Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< FlowTestCase187Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase187Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase187Actor, 3, int >;
friend struct ActorCallback< FlowTestCase187Actor, 4, Void >;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	FlowTestCase187Actor(UnitTestParameters const& params) 
															#line 4287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase187ActorState<FlowTestCase187Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase187", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase187");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase187", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase187Actor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase187Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase187Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase187Actor, 3, int >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< FlowTestCase187Actor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
static Future<Void> flowTestCase187( UnitTestParameters const& params ) {
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
	return Future<Void>(new FlowTestCase187Actor(params));
															#line 4319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase187, "/fileio/truncateAndRead")

#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/IAsyncFile.actor.cpp"
