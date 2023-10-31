#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
#include "fdbrpc/DDSketch.h"
#include "flow/Error.h"
#include "flow/IRandom.h"
#include "flow/UnitTest.h"
#include <limits>
#include <random>
#include "flow/actorcompiler.h" // has to be last include
void forceLinkDDSketchTests() {}

															#line 12 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase10()
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
template <class FlowTestCase10Actor>
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
class FlowTestCase10ActorState {
															#line 19 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
public:
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	FlowTestCase10ActorState(UnitTestParameters const& params) 
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
		 : params(params)
															#line 26 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase10", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase10ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase10", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 12 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			int TRY = 100, SIZE = 1e6;
															#line 13 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			const int totalPercentiles = 7;
															#line 14 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double targetPercentiles[totalPercentiles] = { .0001, .01, .1, .50, .90, .99, .9999 };
															#line 15 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double stat[totalPercentiles] = { 0 };
															#line 16 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			for(int t = 0;t < TRY;t++) {
															#line 17 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				DDSketch<double> dd;
															#line 18 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				std::vector<double> nums;
															#line 19 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				for(int i = 0;i < SIZE;i++) {
															#line 20 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					static double a = 1, b = 1;
															#line 21 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					auto y = deterministicRandom()->random01();
															#line 22 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					auto num = b / pow(1 - y, 1 / a);
															#line 23 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					nums.push_back(num);
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					dd.addSample(num);
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
				}
															#line 26 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				std::sort(nums.begin(), nums.end());
															#line 27 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				for(int percentID = 0;percentID < totalPercentiles;percentID++) {
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					double percentile = targetPercentiles[percentID];
															#line 29 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					double ground = nums[percentile * (SIZE - 1)], ddvalue = dd.percentile(percentile);
															#line 30 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					double relativeError = fabs(ground - ddvalue) / ground;
															#line 31 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
					stat[percentID] += relativeError;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
				}
			}
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			for(int percentID = 0;percentID < totalPercentiles;percentID++) {
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				printf("%.4lf per, relative error %.4lf\n", targetPercentiles[percentID], stat[percentID] / TRY);
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
			}
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			if (!static_cast<FlowTestCase10Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase10ActorState(); static_cast<FlowTestCase10Actor*>(this)->destroy(); return 0; }
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
			new (&static_cast<FlowTestCase10Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase10ActorState();
			static_cast<FlowTestCase10Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase10ActorState();
		static_cast<FlowTestCase10Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	UnitTestParameters params;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase10()
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
class FlowTestCase10Actor final : public Actor<Void>, public FastAllocated<FlowTestCase10Actor>, public FlowTestCase10ActorState<FlowTestCase10Actor> {
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase10Actor>::operator new;
	using FastAllocated<FlowTestCase10Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	FlowTestCase10Actor(UnitTestParameters const& params) 
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase10ActorState<FlowTestCase10Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase10", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase10");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase10", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
static Future<Void> flowTestCase10( UnitTestParameters const& params ) {
															#line 10 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	return Future<Void>(new FlowTestCase10Actor(params));
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase10, "/fdbrpc/ddsketch/accuracy")

#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"

															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase42()
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
template <class FlowTestCase42Actor>
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
class FlowTestCase42ActorState {
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
public:
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	FlowTestCase42ActorState(UnitTestParameters const& params) 
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
		 : params(params)
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase42", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase42ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase42", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			DDSketch<double> dd;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			for(int i = 0;i < 4000;i++) {
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				double sample = (static_cast<double>(deterministicRandom()->randomSkewedUInt32(40, 1000)) / 100000);
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
				dd.addSample(sample);
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
			}
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double p50 = dd.percentile(0.5);
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			ASSERT(p50 > 0 && p50 != std::numeric_limits<double>::infinity());
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double p90 = dd.percentile(0.9);
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			ASSERT(p90 > 0 && p90 != std::numeric_limits<double>::infinity());
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double p95 = dd.percentile(0.95);
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			ASSERT(p95 > 0 && p95 != std::numeric_limits<double>::infinity());
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double p99 = dd.percentile(0.99);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			ASSERT(p99 > 0 && p99 != std::numeric_limits<double>::infinity());
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			double p999 = dd.percentile(0.999);
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			ASSERT(p999 > 0 && p999 != std::numeric_limits<double>::infinity());
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
			if (!static_cast<FlowTestCase42Actor*>(this)->SAV<Void>::futures) { (void)(Void{}); this->~FlowTestCase42ActorState(); static_cast<FlowTestCase42Actor*>(this)->destroy(); return 0; }
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
			new (&static_cast<FlowTestCase42Actor*>(this)->SAV< Void >::value()) Void(Void{});
			this->~FlowTestCase42ActorState();
			static_cast<FlowTestCase42Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase42ActorState();
		static_cast<FlowTestCase42Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	UnitTestParameters params;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase42()
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
class FlowTestCase42Actor final : public Actor<Void>, public FastAllocated<FlowTestCase42Actor>, public FlowTestCase42ActorState<FlowTestCase42Actor> {
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase42Actor>::operator new;
	using FastAllocated<FlowTestCase42Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	FlowTestCase42Actor(UnitTestParameters const& params) 
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase42ActorState<FlowTestCase42Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase42", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase42");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase42", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
static Future<Void> flowTestCase42( UnitTestParameters const& params ) {
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
	return Future<Void>(new FlowTestCase42Actor(params));
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase42, "/fdbrpc/ddsketch/correctness")

#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/DDSketchTest.actor.cpp"
