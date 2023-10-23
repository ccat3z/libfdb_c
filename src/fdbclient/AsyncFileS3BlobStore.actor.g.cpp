#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
/*
 * AsyncFileS3BlobStore.actor.cpp
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

#include "fdbclient/AsyncFileS3BlobStore.actor.h"
#include "fdbrpc/AsyncFileReadAhead.actor.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // has to be last include

Future<int64_t> AsyncFileS3BlobStoreRead::size() const {
	if (!m_size.isValid())
		m_size = m_bstore->objectSize(m_bucket, m_object);
	return m_size;
}

Future<int> AsyncFileS3BlobStoreRead::read(void* data, int length, int64_t offset) {
	return m_bstore->readObject(m_bucket, m_object, data, length, offset);
}

															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
namespace {
// This generated class is to be used only via sendStuff()
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
template <class SendStuffActor>
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
class SendStuffActorState {
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
public:
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	SendStuffActorState(int const& id,Reference<IRateControl> const& t,int const& bytes) 
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		 : id(id),
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		   t(t),
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		   bytes(bytes)
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
	{
		fdb_probe_actor_create("sendStuff", reinterpret_cast<unsigned long>(this));

	}
	~SendStuffActorState() 
	{
		fdb_probe_actor_destroy("sendStuff", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			printf("Starting fake sender %d which will send send %d bytes.\n", id, bytes);
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			ts = timer();
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total = 0;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
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
		this->~SendStuffActorState();
		static_cast<SendStuffActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		double dur = timer() - ts;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		printf("Sender %d: Sent %d in %fs, %f/s\n", id, total, dur, total / dur);
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		if (!static_cast<SendStuffActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendStuffActorState(); static_cast<SendStuffActor*>(this)->destroy(); return 0; }
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		new (&static_cast<SendStuffActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendStuffActorState();
		static_cast<SendStuffActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		if (!(total < bytes))
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		r = std::min<int>(deterministicRandom()->randomInt(0, 1000), bytes - total);
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		StrictFuture<Void> __when_expr_0 = t->getAllowance(r);
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		if (static_cast<SendStuffActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<SendStuffActor*>(this)->actor_wait_state = 1;
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendStuffActor, 0, Void >*>(static_cast<SendStuffActor*>(this)));
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
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
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		total += r;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		total += r;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
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
		if (static_cast<SendStuffActor*>(this)->actor_wait_state > 0) static_cast<SendStuffActor*>(this)->actor_wait_state = 0;
		static_cast<SendStuffActor*>(this)->ActorCallback< SendStuffActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendStuffActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendStuff", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendStuff", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendStuffActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendStuff", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendStuff", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendStuffActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendStuff", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendStuff", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int id;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	Reference<IRateControl> t;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int bytes;
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	double ts;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int total;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int r;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
};
// This generated class is to be used only via sendStuff()
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
class SendStuffActor final : public Actor<Void>, public ActorCallback< SendStuffActor, 0, Void >, public FastAllocated<SendStuffActor>, public SendStuffActorState<SendStuffActor> {
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
public:
	using FastAllocated<SendStuffActor>::operator new;
	using FastAllocated<SendStuffActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendStuffActor, 0, Void >;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	SendStuffActor(int const& id,Reference<IRateControl> const& t,int const& bytes) 
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		 : Actor<Void>(),
		   SendStuffActorState<SendStuffActor>(id, t, bytes)
	{
		fdb_probe_actor_enter("sendStuff", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendStuff");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendStuff", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendStuffActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
[[nodiscard]] Future<Void> sendStuff( int const& id, Reference<IRateControl> const& t, int const& bytes ) {
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	return Future<Void>(new SendStuffActor(id, t, bytes));
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
}

#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"

															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase50()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
template <class FlowTestCase50Actor>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
class FlowTestCase50ActorState {
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
public:
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	FlowTestCase50ActorState(UnitTestParameters const& params) 
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		 : params(params)
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase50", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase50ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase50", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			if (g_network->isSimulated())
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
			{
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
				if (!static_cast<FlowTestCase50Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase50ActorState(); static_cast<FlowTestCase50Actor*>(this)->destroy(); return 0; }
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
				new (&static_cast<FlowTestCase50Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase50ActorState();
				static_cast<FlowTestCase50Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			limit = 100000;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			t = Reference<IRateControl>(new SpeedLimit(limit, 1));
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			id = 1;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			std::vector<Future<Void>> f;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			ts = timer();
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total = 0;
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			int s;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			s = 500000;
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			f.push_back(sendStuff(id++, t, s));
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total += s;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			f.push_back(sendStuff(id++, t, s));
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total += s;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			s = 50000;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			f.push_back(sendStuff(id++, t, s));
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total += s;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			f.push_back(sendStuff(id++, t, s));
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total += s;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			s = 5000;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			f.push_back(sendStuff(id++, t, s));
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			total += s;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(f);
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			if (static_cast<FlowTestCase50Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase50Actor*>(this)->actor_wait_state = 1;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase50Actor, 0, Void >*>(static_cast<FlowTestCase50Actor*>(this)));
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
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
		this->~FlowTestCase50ActorState();
		static_cast<FlowTestCase50Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		double dur = timer() - ts;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		int speed = int(total / dur);
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		printf("Speed limit was %d, measured speed was %d\n", limit, speed);
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		ASSERT(abs(speed - limit) / limit < .01);
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		if (!static_cast<FlowTestCase50Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase50ActorState(); static_cast<FlowTestCase50Actor*>(this)->destroy(); return 0; }
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		new (&static_cast<FlowTestCase50Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase50ActorState();
		static_cast<FlowTestCase50Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		double dur = timer() - ts;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		int speed = int(total / dur);
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		printf("Speed limit was %d, measured speed was %d\n", limit, speed);
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		ASSERT(abs(speed - limit) / limit < .01);
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
		if (!static_cast<FlowTestCase50Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase50ActorState(); static_cast<FlowTestCase50Actor*>(this)->destroy(); return 0; }
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		new (&static_cast<FlowTestCase50Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase50ActorState();
		static_cast<FlowTestCase50Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase50Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase50Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase50Actor*>(this)->ActorCallback< FlowTestCase50Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase50Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase50", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase50", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase50Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase50", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase50", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase50Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase50", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase50", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	UnitTestParameters params;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int limit;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	Reference<IRateControl> t;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int id;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	double ts;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	int total;
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase50()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
class FlowTestCase50Actor final : public Actor<Void>, public ActorCallback< FlowTestCase50Actor, 0, Void >, public FastAllocated<FlowTestCase50Actor>, public FlowTestCase50ActorState<FlowTestCase50Actor> {
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase50Actor>::operator new;
	using FastAllocated<FlowTestCase50Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase50Actor, 0, Void >;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	FlowTestCase50Actor(UnitTestParameters const& params) 
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase50ActorState<FlowTestCase50Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase50", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase50");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase50", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase50Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
static Future<Void> flowTestCase50( UnitTestParameters const& params ) {
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
	return Future<Void>(new FlowTestCase50Actor(params));
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase50, "/backup/throttling")

#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.cpp"
