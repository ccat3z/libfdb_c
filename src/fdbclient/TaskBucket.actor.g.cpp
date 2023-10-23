#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
/*
 * TaskBucket.actor.cpp
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

#include "fdbclient/TaskBucket.h"
#include "fdbclient/ReadYourWrites.h"
#include "flow/actorcompiler.h" // has to be last include

FDB_DEFINE_BOOLEAN_PARAM(AccessSystemKeys);
FDB_DEFINE_BOOLEAN_PARAM(PriorityBatch);
FDB_DEFINE_BOOLEAN_PARAM(VerifyTask);
FDB_DEFINE_BOOLEAN_PARAM(UpdateParams);

Reference<TaskFuture> Task::getDoneFuture(Reference<FutureBucket> fb) {
	return fb->unpack(params[reservedTaskParamKeyDone]);
}

struct UnblockFutureTaskFunc : TaskFuncBase {
	static StringRef name;

	StringRef getName() const override { return name; };
	Future<Void> execute(Database cx,
	                     Reference<TaskBucket> tb,
	                     Reference<FutureBucket> fb,
	                     Reference<Task> task) override {
		return Void();
	};
	Future<Void> finish(Reference<ReadYourWritesTransaction> tr,
	                    Reference<TaskBucket> tb,
	                    Reference<FutureBucket> fb,
	                    Reference<Task> task) override {
		return _finish(tr, tb, fb, task);
	};

																#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via _finish()
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class _finishActor>
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _finishActorState {
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_finishActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   future(futureBucket->unpack(task->params[Task::reservedTaskParamKeyFuture]))
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("_finish", reinterpret_cast<unsigned long>(this));

	}
	~_finishActorState() 
	{
		fdb_probe_actor_destroy("_finish", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			futureBucket->setOptions(tr);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->clear(future->blocks.pack(task->params[Task::reservedTaskParamKeyBlockID]));
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = future->isSet(tr);
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_finishActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_finishActor*>(this)->actor_wait_state = 1;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _finishActor, 0, bool >*>(static_cast<_finishActor*>(this)));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~_finishActorState();
		static_cast<_finishActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& is_set,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = future->performAllActions(tr, taskBucket);
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_finishActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<_finishActor*>(this)->actor_wait_state = 2;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _finishActor, 1, Void >*>(static_cast<_finishActor*>(this)));
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(bool && is_set,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = future->performAllActions(tr, taskBucket);
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_finishActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<_finishActor*>(this)->actor_wait_state = 2;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _finishActor, 1, Void >*>(static_cast<_finishActor*>(this)));
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(bool const& is_set,int loopDepth) 
	{
		loopDepth = a_body1cont1(is_set, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && is_set,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(is_set), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_finishActor*>(this)->actor_wait_state > 0) static_cast<_finishActor*>(this)->actor_wait_state = 0;
		static_cast<_finishActor*>(this)->ActorCallback< _finishActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< _finishActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _finishActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _finishActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<_finishActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_finishActorState(); static_cast<_finishActor*>(this)->destroy(); return 0; }
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<_finishActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_finishActorState();
		static_cast<_finishActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

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
		if (static_cast<_finishActor*>(this)->actor_wait_state > 0) static_cast<_finishActor*>(this)->actor_wait_state = 0;
		static_cast<_finishActor*>(this)->ActorCallback< _finishActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< _finishActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< _finishActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< _finishActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> future;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via _finish()
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _finishActor final : public Actor<Void>, public ActorCallback< _finishActor, 0, bool >, public ActorCallback< _finishActor, 1, Void >, public FastAllocated<_finishActor>, public _finishActorState<_finishActor> {
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<_finishActor>::operator new;
	using FastAllocated<_finishActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _finishActor, 0, bool >;
friend struct ActorCallback< _finishActor, 1, Void >;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_finishActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   _finishActorState<_finishActor>(tr, taskBucket, futureBucket, task)
	{
		fdb_probe_actor_enter("_finish", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_finish");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_finish", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _finishActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< _finishActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> _finish( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, Reference<Task> const& task ) {
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new _finishActor(tr, taskBucket, futureBucket, task));
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};
StringRef UnblockFutureTaskFunc::name = LiteralStringRef("UnblockFuture");
REGISTER_TASKFUNC(UnblockFutureTaskFunc);

struct AddTaskFunc : TaskFuncBase {
	static StringRef name;

	StringRef getName() const override { return name; };
	Future<Void> execute(Database cx,
	                     Reference<TaskBucket> tb,
	                     Reference<FutureBucket> fb,
	                     Reference<Task> task) override {
		return Void();
	};
	Future<Void> finish(Reference<ReadYourWritesTransaction> tr,
	                    Reference<TaskBucket> tb,
	                    Reference<FutureBucket> fb,
	                    Reference<Task> task) override {
		task->params[Task::reservedTaskParamKeyType] = task->params[Task::reservedTaskParamKeyAddTask];
		tb->addTask(tr, task);
		return Void();
	};
};
StringRef AddTaskFunc::name = LiteralStringRef("AddTask");
REGISTER_TASKFUNC(AddTaskFunc);

struct IdleTaskFunc : TaskFuncBase {
	static StringRef name;
	static constexpr uint32_t version = 1;

	StringRef getName() const override { return name; };
	Future<Void> execute(Database cx,
	                     Reference<TaskBucket> tb,
	                     Reference<FutureBucket> fb,
	                     Reference<Task> task) override {
		return Void();
	};
	Future<Void> finish(Reference<ReadYourWritesTransaction> tr,
	                    Reference<TaskBucket> tb,
	                    Reference<FutureBucket> fb,
	                    Reference<Task> task) override {
		return tb->finish(tr, task);
	};
};
StringRef IdleTaskFunc::name = LiteralStringRef("idle");
REGISTER_TASKFUNC(IdleTaskFunc);

Key Task::reservedTaskParamKeyType = LiteralStringRef("type");
Key Task::reservedTaskParamKeyAddTask = LiteralStringRef("_add_task");
Key Task::reservedTaskParamKeyDone = LiteralStringRef("done");
Key Task::reservedTaskParamKeyPriority = LiteralStringRef("priority");
Key Task::reservedTaskParamKeyFuture = LiteralStringRef("future");
Key Task::reservedTaskParamKeyBlockID = LiteralStringRef("blockid");
Key Task::reservedTaskParamKeyVersion = LiteralStringRef("version");
Key Task::reservedTaskParamValidKey = LiteralStringRef("_validkey");
Key Task::reservedTaskParamValidValue = LiteralStringRef("_validvalue");

// IMPORTANT:  Task() must result in an EMPTY parameter set, so params should only
// be set for non-default constructor arguments.  To change this behavior look at all
// Task() default constructions to see if they require params to be empty and call clear.
Task::Task(Value type, uint32_t version, Value done, unsigned int priority) : extendMutex(1) {
	if (type.size())
		params[Task::reservedTaskParamKeyType] = type;

	if (version > 0)
		params[Task::reservedTaskParamKeyVersion] = BinaryWriter::toValue(version, Unversioned());

	if (done.size())
		params[Task::reservedTaskParamKeyDone] = done;

	priority = std::min<int64_t>(priority, CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY);
	if (priority != 0)
		params[Task::reservedTaskParamKeyPriority] = BinaryWriter::toValue<int64_t>(priority, Unversioned());
}

uint32_t Task::getVersion() const {
	uint32_t version(0);
	auto itor = params.find(Task::reservedTaskParamKeyVersion);
	if (itor != params.end()) {
		version = BinaryReader::fromStringRef<uint32_t>(itor->value, Unversioned());
	} else {
		TraceEvent(SevWarn, "InvalidTaskVersion").detail("TaskHasNoVersion", version);
	}

	return version;
}

unsigned int Task::getPriority() const {
	unsigned int priority = 0;
	auto i = params.find(Task::reservedTaskParamKeyPriority);
	if (i != params.end())
		priority = std::min<int64_t>(BinaryReader::fromStringRef<int64_t>(i->value, Unversioned()),
		                             CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY);
	return priority;
}

class TaskBucketImpl {
public:
																#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getTaskKey()
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetTaskKeyActor>
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskKeyActorState {
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskKeyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,int const& priority = 0) 
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   priority(priority)
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("getTaskKey", reinterpret_cast<unsigned long>(this));

	}
	~GetTaskKeyActorState() 
	{
		fdb_probe_actor_destroy("getTaskKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Standalone<StringRef> uid = StringRef(deterministicRandom()->randomUniqueID().toString());
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			space = taskBucket->getAvailableSpace(priority);
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				StrictFuture<Key> __when_expr_0 = tr->getKey(lastLessOrEqual(space.pack(uid)), Snapshot::True);
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 1;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTaskKeyActor, 0, Key >*>(static_cast<GetTaskKeyActor*>(this)));
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				loopDepth = 0;
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
		this->~GetTaskKeyActorState();
		static_cast<GetTaskKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Key> __when_expr_1 = tr->getKey(lastLessOrEqual(space.pack(maxUIDKey)), Snapshot::True);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 2;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetTaskKeyActor, 1, Key >*>(static_cast<GetTaskKeyActor*>(this)));
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2(Key const& k,int loopDepth) 
	{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (space.contains(k))
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(k)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(k));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Key && k,int loopDepth) 
	{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (space.contains(k))
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(k)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(k));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key const& k,int loopDepth) 
	{
		loopDepth = a_body1cont2(k, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && k,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(k), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state > 0) static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetTaskKeyActor*>(this)->ActorCallback< GetTaskKeyActor, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTaskKeyActor, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>()); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>());
		this->~GetTaskKeyActorState();
		static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Key const& k,int loopDepth) 
	{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (space.contains(k))
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(k)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(k));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Key && k,int loopDepth) 
	{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (space.contains(k))
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(k)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(k));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Key const& k,int loopDepth) 
	{
		loopDepth = a_body1cont6(k, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Key && k,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(k), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state > 0) static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetTaskKeyActor*>(this)->ActorCallback< GetTaskKeyActor, 1, Key >::remove();

	}
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 1, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 1, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetTaskKeyActor, 1, Key >*,Error err) 
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int priority;
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace space;
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getTaskKey()
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskKeyActor final : public Actor<Optional<Key>>, public ActorCallback< GetTaskKeyActor, 0, Key >, public ActorCallback< GetTaskKeyActor, 1, Key >, public FastAllocated<GetTaskKeyActor>, public GetTaskKeyActorState<GetTaskKeyActor> {
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetTaskKeyActor>::operator new;
	using FastAllocated<GetTaskKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Key>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTaskKeyActor, 0, Key >;
friend struct ActorCallback< GetTaskKeyActor, 1, Key >;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskKeyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,int const& priority = 0) 
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Optional<Key>>(),
		   GetTaskKeyActorState<GetTaskKeyActor>(tr, taskBucket, priority)
	{
		fdb_probe_actor_enter("getTaskKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTaskKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTaskKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTaskKeyActor, 0, Key >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetTaskKeyActor, 1, Key >*)0, actor_cancelled()); break;
		}

	}
};
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Optional<Key>> getTaskKey( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, int const& priority = 0 ) {
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Optional<Key>>(new GetTaskKeyActor(tr, taskBucket, priority));
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getOne()
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetOneActor>
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetOneActorState {
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetOneActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("getOne", reinterpret_cast<unsigned long>(this));

	}
	~GetOneActorState() 
	{
		fdb_probe_actor_destroy("getOne", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (taskBucket->priority_batch)
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->setOption(FDBTransactionOptions::PRIORITY_BATCH);
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (deterministicRandom()->random01() < CLIENT_KNOBS->TASKBUCKET_CHECK_TIMEOUT_CHANCE)
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				StrictFuture<bool> __when_expr_0 = requeueTimedOutTasks(tr, taskBucket);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetOneActor*>(this)->actor_wait_state = 1;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 0, bool >*>(static_cast<GetOneActor*>(this)));
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskKeyFutures = std::vector<Future<Optional<Key>>>(CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY + 1);
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		pri = int();
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(pri = CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;pri >= 0;--pri) {
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKeyFutures[pri] = getTaskKey(tr, taskBucket, pri);
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskKey = Optional<Key>();
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		availableSpace = Subspace();
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		pri = CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool const& anyTimeouts,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TEST(anyTimeouts);
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool && anyTimeouts,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TEST(anyTimeouts);
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool const& anyTimeouts,int loopDepth) 
	{
		loopDepth = a_body1cont3(anyTimeouts, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && anyTimeouts,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(anyTimeouts), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetOneActor*>(this)->actor_wait_state > 0) static_cast<GetOneActor*>(this)->actor_wait_state = 0;
		static_cast<GetOneActor*>(this)->ActorCallback< GetOneActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< GetOneActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetOneActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetOneActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!taskKey.present())
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_2 = requeueTimedOutTasks(tr, taskBucket);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 3;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 2, bool >*>(static_cast<GetOneActor*>(this)));
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(pri >= 0))
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (taskKey.present())
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKeyFutures[pri].cancel();
															#line 1052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}
		else
		{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Key>> __when_expr_1 = taskKeyFutures[pri];
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 2;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 1, Optional<Key> >*>(static_cast<GetOneActor*>(this)));
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		--pri;
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Optional<Key> const& key,int loopDepth) 
	{
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (key.present())
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKey = key;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSpace = taskBucket->getAvailableSpace(pri);
															#line 1104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Optional<Key> && key,int loopDepth) 
	{
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (key.present())
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKey = key;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSpace = taskBucket->getAvailableSpace(pri);
															#line 1120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Optional<Key> const& key,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont4(key, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Optional<Key> && key,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont4(std::move(key), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetOneActor*>(this)->actor_wait_state > 0) static_cast<GetOneActor*>(this)->actor_wait_state = 0;
		static_cast<GetOneActor*>(this)->ActorCallback< GetOneActor, 1, Optional<Key> >::remove();

	}
	void a_callback_fire(ActorCallback< GetOneActor, 1, Optional<Key> >*,Optional<Key> const& value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetOneActor, 1, Optional<Key> >*,Optional<Key> && value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetOneActor, 1, Optional<Key> >*,Error err) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(int loopDepth) 
	{
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		t = availableSpace.unpack(taskKey.get());
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskUID = t.getString(0);
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskAvailableSpace = availableSpace.get(taskUID);
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task = Reference<Task>(new Task());
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->key = taskUID;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_4 = tr->getRange(taskAvailableSpace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont7when1(__when_expr_4.get(), loopDepth); };
		static_cast<GetOneActor*>(this)->actor_wait_state = 5;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 4, RangeResult >*>(static_cast<GetOneActor*>(this)));
															#line 1210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont8(bool const& anyTimeouts,int loopDepth) 
	{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (anyTimeouts)
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<Task>> __when_expr_3 = getOne(tr, taskBucket);
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont8when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 4;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 3, Reference<Task> >*>(static_cast<GetOneActor*>(this)));
															#line 1232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont9(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(bool && anyTimeouts,int loopDepth) 
	{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (anyTimeouts)
															#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<Task>> __when_expr_3 = getOne(tr, taskBucket);
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont8when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 4;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 3, Reference<Task> >*>(static_cast<GetOneActor*>(this)));
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont9(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5when1(bool const& anyTimeouts,int loopDepth) 
	{
		loopDepth = a_body1cont8(anyTimeouts, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(bool && anyTimeouts,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(anyTimeouts), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetOneActor*>(this)->actor_wait_state > 0) static_cast<GetOneActor*>(this)->actor_wait_state = 0;
		static_cast<GetOneActor*>(this)->ActorCallback< GetOneActor, 2, bool >::remove();

	}
	void a_callback_fire(ActorCallback< GetOneActor, 2, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetOneActor, 2, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetOneActor, 2, bool >*,Error err) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont9(int loopDepth) 
	{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(Reference<Task>()); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(Reference<Task>());
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(Reference<Task> const& task,int loopDepth) 
	{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(task);
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(Reference<Task> && task,int loopDepth) 
	{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(task);
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8when1(Reference<Task> const& task,int loopDepth) 
	{
		loopDepth = a_body1cont10(task, loopDepth);

		return loopDepth;
	}
	int a_body1cont8when1(Reference<Task> && task,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(task), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<GetOneActor*>(this)->actor_wait_state > 0) static_cast<GetOneActor*>(this)->actor_wait_state = 0;
		static_cast<GetOneActor*>(this)->ActorCallback< GetOneActor, 3, Reference<Task> >::remove();

	}
	void a_callback_fire(ActorCallback< GetOneActor, 3, Reference<Task> >*,Reference<Task> const& value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetOneActor, 3, Reference<Task> >*,Reference<Task> && value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetOneActor, 3, Reference<Task> >*,Error err) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont12(int loopDepth) 
	{
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Version> __when_expr_5 = tr->getReadVersion();
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont12when1(__when_expr_5.get(), loopDepth); };
		static_cast<GetOneActor*>(this)->actor_wait_state = 6;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 5, Version >*>(static_cast<GetOneActor*>(this)));
															#line 1442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7when1(RangeResult const& __values,int loopDepth) 
	{
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		values = __values;
															#line 1451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(RangeResult && __values,int loopDepth) 
	{
		values = std::move(__values);
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<GetOneActor*>(this)->actor_wait_state > 0) static_cast<GetOneActor*>(this)->actor_wait_state = 0;
		static_cast<GetOneActor*>(this)->ActorCallback< GetOneActor, 4, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetOneActor, 4, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< GetOneActor, 4, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< GetOneActor, 4, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont12cont1(Version const& version,int loopDepth) 
	{
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = version + (uint64_t)(taskBucket->timeout * (CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_OFFSET + CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_RANGE * deterministicRandom()->random01()));
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Subspace timeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(taskUID);
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& s : values ) {
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = taskAvailableSpace.unpack(s.key).getString(0);
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[param] = s.value;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(timeoutSpace.pack(param), s.value);
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskAvailableSpace.range());
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->set(taskBucket->active.key(), deterministicRandom()->randomUniqueID().toString());
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(std::move(task)); // state_var_RVO
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12cont1(Version && version,int loopDepth) 
	{
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = version + (uint64_t)(taskBucket->timeout * (CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_OFFSET + CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_RANGE * deterministicRandom()->random01()));
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Subspace timeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(taskUID);
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& s : values ) {
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = taskAvailableSpace.unpack(s.key).getString(0);
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[param] = s.value;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(timeoutSpace.pack(param), s.value);
															#line 1558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskAvailableSpace.range());
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->set(taskBucket->active.key(), deterministicRandom()->randomUniqueID().toString());
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(std::move(task)); // state_var_RVO
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12when1(Version const& version,int loopDepth) 
	{
		loopDepth = a_body1cont12cont1(version, loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Version && version,int loopDepth) 
	{
		loopDepth = a_body1cont12cont1(std::move(version), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<GetOneActor*>(this)->actor_wait_state > 0) static_cast<GetOneActor*>(this)->actor_wait_state = 0;
		static_cast<GetOneActor*>(this)->ActorCallback< GetOneActor, 5, Version >::remove();

	}
	void a_callback_fire(ActorCallback< GetOneActor, 5, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont12when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< GetOneActor, 5, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont12when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< GetOneActor, 5, Version >*,Error err) 
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<Optional<Key>>> taskKeyFutures;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int pri;
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Optional<Key> taskKey;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace availableSpace;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Tuple t;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Key taskUID;
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace taskAvailableSpace;
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RangeResult values;
															#line 1659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getOne()
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetOneActor final : public Actor<Reference<Task>>, public ActorCallback< GetOneActor, 0, bool >, public ActorCallback< GetOneActor, 1, Optional<Key> >, public ActorCallback< GetOneActor, 2, bool >, public ActorCallback< GetOneActor, 3, Reference<Task> >, public ActorCallback< GetOneActor, 4, RangeResult >, public ActorCallback< GetOneActor, 5, Version >, public FastAllocated<GetOneActor>, public GetOneActorState<GetOneActor> {
															#line 1664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetOneActor>::operator new;
	using FastAllocated<GetOneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<Task>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetOneActor, 0, bool >;
friend struct ActorCallback< GetOneActor, 1, Optional<Key> >;
friend struct ActorCallback< GetOneActor, 2, bool >;
friend struct ActorCallback< GetOneActor, 3, Reference<Task> >;
friend struct ActorCallback< GetOneActor, 4, RangeResult >;
friend struct ActorCallback< GetOneActor, 5, Version >;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetOneActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 1680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Reference<Task>>(),
		   GetOneActorState<GetOneActor>(tr, taskBucket)
	{
		fdb_probe_actor_enter("getOne", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getOne");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getOne", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetOneActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetOneActor, 1, Optional<Key> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetOneActor, 2, bool >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GetOneActor, 3, Reference<Task> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< GetOneActor, 4, RangeResult >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< GetOneActor, 5, Version >*)0, actor_cancelled()); break;
		}

	}
};
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Reference<Task>> getOne( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Reference<Task>>(new GetOneActor(tr, taskBucket));
															#line 1712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	// Verify that the user configured task verification key still has the user specified value
																#line 1718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via taskVerify()
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class TaskVerifyActor>
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActorState {
															#line 1724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActorState(Reference<TaskBucket> const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task) 
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tb(tb),
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(tr),
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 1735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("taskVerify", reinterpret_cast<unsigned long>(this));

	}
	~TaskVerifyActorState() 
	{
		fdb_probe_actor_destroy("taskVerify", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task->params.find(Task::reservedTaskParamValidKey) == task->params.end())
															#line 1750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", "missing");
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
				this->~TaskVerifyActorState();
				static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task->params.find(Task::reservedTaskParamValidValue) == task->params.end())
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", "missing");
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
				this->~TaskVerifyActorState();
				static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tb->setOptions(tr);
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(task->params[Task::reservedTaskParamValidKey]);
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<TaskVerifyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TaskVerifyActor*>(this)->actor_wait_state = 1;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TaskVerifyActor, 0, Optional<Value> >*>(static_cast<TaskVerifyActor*>(this)));
															#line 1787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~TaskVerifyActorState();
		static_cast<TaskVerifyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& keyValue,int loopDepth) 
	{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!keyValue.present())
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", "missing");
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (keyValue.get().compare(StringRef(task->params[Task::reservedTaskParamValidValue])))
															#line 1824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyAbortedTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", keyValue.get());
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~TaskVerifyActorState();
		static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && keyValue,int loopDepth) 
	{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!keyValue.present())
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", "missing");
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (keyValue.get().compare(StringRef(task->params[Task::reservedTaskParamValidValue])))
															#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyAbortedTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", keyValue.get());
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~TaskVerifyActorState();
		static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& keyValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(keyValue, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && keyValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(keyValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TaskVerifyActor*>(this)->actor_wait_state > 0) static_cast<TaskVerifyActor*>(this)->actor_wait_state = 0;
		static_cast<TaskVerifyActor*>(this)->ActorCallback< TaskVerifyActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< TaskVerifyActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TaskVerifyActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TaskVerifyActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> tb;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 1955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via taskVerify()
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActor final : public Actor<bool>, public ActorCallback< TaskVerifyActor, 0, Optional<Value> >, public FastAllocated<TaskVerifyActor>, public TaskVerifyActorState<TaskVerifyActor> {
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<TaskVerifyActor>::operator new;
	using FastAllocated<TaskVerifyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TaskVerifyActor, 0, Optional<Value> >;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActor(Reference<TaskBucket> const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task) 
															#line 1971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   TaskVerifyActorState<TaskVerifyActor>(tb, tr, task)
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("taskVerify");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TaskVerifyActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> taskVerify( Reference<TaskBucket> const& tb, Reference<ReadYourWritesTransaction> const& tr, Reference<Task> const& task ) {
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new TaskVerifyActor(tb, tr, task));
															#line 1998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 2003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via taskVerify()
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class TaskVerifyActor1>
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActor1State {
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActor1State(Reference<TaskBucket> const& tb,Database const& cx,Reference<Task> const& task) 
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tb(tb),
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   cx(cx),
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 2020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("taskVerify", reinterpret_cast<unsigned long>(this));

	}
	~TaskVerifyActor1State() 
	{
		fdb_probe_actor_destroy("taskVerify", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 2035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~TaskVerifyActor1State();
		static_cast<TaskVerifyActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 2065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		try {
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = taskVerify(tb, tr, task);
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<TaskVerifyActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TaskVerifyActor1*>(this)->actor_wait_state = 1;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TaskVerifyActor1, 0, bool >*>(static_cast<TaskVerifyActor1*>(this)));
															#line 2076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<TaskVerifyActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TaskVerifyActor1*>(this)->actor_wait_state = 2;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TaskVerifyActor1, 1, Void >*>(static_cast<TaskVerifyActor1*>(this)));
															#line 2105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& verified,int loopDepth) 
	{
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor1*>(this)->SAV<bool>::futures) { (void)(verified); this->~TaskVerifyActor1State(); static_cast<TaskVerifyActor1*>(this)->destroy(); return 0; }
															#line 2120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<TaskVerifyActor1*>(this)->SAV< bool >::value()) bool(verified);
		this->~TaskVerifyActor1State();
		static_cast<TaskVerifyActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && verified,int loopDepth) 
	{
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor1*>(this)->SAV<bool>::futures) { (void)(verified); this->~TaskVerifyActor1State(); static_cast<TaskVerifyActor1*>(this)->destroy(); return 0; }
															#line 2132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<TaskVerifyActor1*>(this)->SAV< bool >::value()) bool(verified);
		this->~TaskVerifyActor1State();
		static_cast<TaskVerifyActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& verified,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(verified, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && verified,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(verified), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TaskVerifyActor1*>(this)->actor_wait_state > 0) static_cast<TaskVerifyActor1*>(this)->actor_wait_state = 0;
		static_cast<TaskVerifyActor1*>(this)->ActorCallback< TaskVerifyActor1, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< TaskVerifyActor1, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TaskVerifyActor1, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TaskVerifyActor1, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 0);

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
	void a_exitChoose2() 
	{
		if (static_cast<TaskVerifyActor1*>(this)->actor_wait_state > 0) static_cast<TaskVerifyActor1*>(this)->actor_wait_state = 0;
		static_cast<TaskVerifyActor1*>(this)->ActorCallback< TaskVerifyActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TaskVerifyActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TaskVerifyActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TaskVerifyActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> tb;
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 2286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via taskVerify()
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActor1 final : public Actor<bool>, public ActorCallback< TaskVerifyActor1, 0, bool >, public ActorCallback< TaskVerifyActor1, 1, Void >, public FastAllocated<TaskVerifyActor1>, public TaskVerifyActor1State<TaskVerifyActor1> {
															#line 2291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<TaskVerifyActor1>::operator new;
	using FastAllocated<TaskVerifyActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TaskVerifyActor1, 0, bool >;
friend struct ActorCallback< TaskVerifyActor1, 1, Void >;
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActor1(Reference<TaskBucket> const& tb,Database const& cx,Reference<Task> const& task) 
															#line 2303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   TaskVerifyActor1State<TaskVerifyActor1>(tb, cx, task)
	{
		fdb_probe_actor_enter("taskVerify", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("taskVerify");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("taskVerify", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TaskVerifyActor1, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TaskVerifyActor1, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> taskVerify( Reference<TaskBucket> const& tb, Database const& cx, Reference<Task> const& task ) {
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new TaskVerifyActor1(tb, cx, task));
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via finishTaskRun()
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class FinishTaskRunActor>
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class FinishTaskRunActorState {
															#line 2342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	FinishTaskRunActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task,Reference<TaskFuncBase> const& taskFunc,VerifyTask const& verifyTask) 
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFunc(taskFunc),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   verifyTask(verifyTask)
															#line 2359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("finishTaskRun", reinterpret_cast<unsigned long>(this));

	}
	~FinishTaskRunActorState() 
	{
		fdb_probe_actor_destroy("finishTaskRun", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = taskBucket->isFinished(tr, task);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 1;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 0, bool >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~FinishTaskRunActorState();
		static_cast<FinishTaskRunActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& isFinished,int loopDepth) 
	{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isFinished)
															#line 2404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<FinishTaskRunActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FinishTaskRunActorState(); static_cast<FinishTaskRunActor*>(this)->destroy(); return 0; }
															#line 2408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<FinishTaskRunActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FinishTaskRunActorState();
			static_cast<FinishTaskRunActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = true;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (verifyTask)
															#line 2418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskVerify(taskBucket, tr, task);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 2;
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 1, bool >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(bool && isFinished,int loopDepth) 
	{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isFinished)
															#line 2443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<FinishTaskRunActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FinishTaskRunActorState(); static_cast<FinishTaskRunActor*>(this)->destroy(); return 0; }
															#line 2447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<FinishTaskRunActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FinishTaskRunActorState();
			static_cast<FinishTaskRunActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = true;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (verifyTask)
															#line 2457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskVerify(taskBucket, tr, task);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 2;
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 1, bool >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(bool const& isFinished,int loopDepth) 
	{
		loopDepth = a_body1cont1(isFinished, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && isFinished,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(isFinished), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state > 0) static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 0;
		static_cast<FinishTaskRunActor*>(this)->ActorCallback< FinishTaskRunActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FinishTaskRunActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validTask)
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_2 = taskBucket->finish(tr, task);
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 3;
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 2, Void >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_3 = taskFunc->finish(tr, taskBucket, futureBucket, task);
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont2when2(__when_expr_3.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 4;
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 3, Void >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont4(bool const& _validTask,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = _validTask;
															#line 2580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(bool && _validTask,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = _validTask;
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool const& _validTask,int loopDepth) 
	{
		loopDepth = a_body1cont4(_validTask, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool && _validTask,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_validTask), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state > 0) static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 0;
		static_cast<FinishTaskRunActor*>(this)->ActorCallback< FinishTaskRunActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FinishTaskRunActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<FinishTaskRunActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FinishTaskRunActorState(); static_cast<FinishTaskRunActor*>(this)->destroy(); return 0; }
															#line 2661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<FinishTaskRunActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FinishTaskRunActorState();
		static_cast<FinishTaskRunActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state > 0) static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 0;
		static_cast<FinishTaskRunActor*>(this)->ActorCallback< FinishTaskRunActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FinishTaskRunActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state > 0) static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 0;
		static_cast<FinishTaskRunActor*>(this)->ActorCallback< FinishTaskRunActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FinishTaskRunActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FinishTaskRunActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuncBase> taskFunc;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	VerifyTask verifyTask;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	bool validTask;
															#line 2833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via finishTaskRun()
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class FinishTaskRunActor final : public Actor<Void>, public ActorCallback< FinishTaskRunActor, 0, bool >, public ActorCallback< FinishTaskRunActor, 1, bool >, public ActorCallback< FinishTaskRunActor, 2, Void >, public ActorCallback< FinishTaskRunActor, 3, Void >, public FastAllocated<FinishTaskRunActor>, public FinishTaskRunActorState<FinishTaskRunActor> {
															#line 2838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<FinishTaskRunActor>::operator new;
	using FastAllocated<FinishTaskRunActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FinishTaskRunActor, 0, bool >;
friend struct ActorCallback< FinishTaskRunActor, 1, bool >;
friend struct ActorCallback< FinishTaskRunActor, 2, Void >;
friend struct ActorCallback< FinishTaskRunActor, 3, Void >;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	FinishTaskRunActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task,Reference<TaskFuncBase> const& taskFunc,VerifyTask const& verifyTask) 
															#line 2852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   FinishTaskRunActorState<FinishTaskRunActor>(tr, taskBucket, futureBucket, task, taskFunc, verifyTask)
	{
		fdb_probe_actor_enter("finishTaskRun", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("finishTaskRun");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("finishTaskRun", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FinishTaskRunActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FinishTaskRunActor, 1, bool >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FinishTaskRunActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FinishTaskRunActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> finishTaskRun( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, Reference<Task> const& task, Reference<TaskFuncBase> const& taskFunc, VerifyTask const& verifyTask ) {
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new FinishTaskRunActor(tr, taskBucket, futureBucket, task, taskFunc, verifyTask));
															#line 2882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 2887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via doOne()
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DoOneActor>
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoOneActorState {
															#line 2893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoOneActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket) 
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket)
															#line 2904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("doOne", reinterpret_cast<unsigned long>(this));

	}
	~DoOneActorState() 
	{
		fdb_probe_actor_destroy("doOne", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<Task>> __when_expr_0 = taskBucket->getOne(cx);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoOneActor*>(this)->actor_wait_state = 1;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoOneActor, 0, Reference<Task> >*>(static_cast<DoOneActor*>(this)));
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~DoOneActorState();
		static_cast<DoOneActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<bool> __when_expr_1 = taskBucket->doTask(cx, futureBucket, task);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoOneActor*>(this)->actor_wait_state = 2;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoOneActor, 1, bool >*>(static_cast<DoOneActor*>(this)));
															#line 2956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<Task> const& __task,int loopDepth) 
	{
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task = __task;
															#line 2965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<Task> && __task,int loopDepth) 
	{
		task = std::move(__task);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoOneActor*>(this)->actor_wait_state > 0) static_cast<DoOneActor*>(this)->actor_wait_state = 0;
		static_cast<DoOneActor*>(this)->ActorCallback< DoOneActor, 0, Reference<Task> >::remove();

	}
	void a_callback_fire(ActorCallback< DoOneActor, 0, Reference<Task> >*,Reference<Task> const& value) 
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoOneActor, 0, Reference<Task> >*,Reference<Task> && value) 
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoOneActor, 0, Reference<Task> >*,Error err) 
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(bool const& result,int loopDepth) 
	{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoOneActor*>(this)->SAV<bool>::futures) { (void)(result); this->~DoOneActorState(); static_cast<DoOneActor*>(this)->destroy(); return 0; }
															#line 3032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoOneActor*>(this)->SAV< bool >::value()) bool(result);
		this->~DoOneActorState();
		static_cast<DoOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(bool && result,int loopDepth) 
	{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoOneActor*>(this)->SAV<bool>::futures) { (void)(result); this->~DoOneActorState(); static_cast<DoOneActor*>(this)->destroy(); return 0; }
															#line 3044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoOneActor*>(this)->SAV< bool >::value()) bool(result);
		this->~DoOneActorState();
		static_cast<DoOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(bool const& result,int loopDepth) 
	{
		loopDepth = a_body1cont2(result, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool && result,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoOneActor*>(this)->actor_wait_state > 0) static_cast<DoOneActor*>(this)->actor_wait_state = 0;
		static_cast<DoOneActor*>(this)->ActorCallback< DoOneActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< DoOneActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoOneActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoOneActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 3123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via doOne()
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoOneActor final : public Actor<bool>, public ActorCallback< DoOneActor, 0, Reference<Task> >, public ActorCallback< DoOneActor, 1, bool >, public FastAllocated<DoOneActor>, public DoOneActorState<DoOneActor> {
															#line 3128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DoOneActor>::operator new;
	using FastAllocated<DoOneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoOneActor, 0, Reference<Task> >;
friend struct ActorCallback< DoOneActor, 1, bool >;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoOneActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket) 
															#line 3140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   DoOneActorState<DoOneActor>(cx, taskBucket, futureBucket)
	{
		fdb_probe_actor_enter("doOne", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doOne");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doOne", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoOneActor, 0, Reference<Task> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoOneActor, 1, bool >*)0, actor_cancelled()); break;
		}

	}
};
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> doOne( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket ) {
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new DoOneActor(cx, taskBucket, futureBucket));
															#line 3168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 3173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via extendTimeoutRepeatedly()
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class ExtendTimeoutRepeatedlyActor>
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutRepeatedlyActorState {
															#line 3179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutRepeatedlyActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx)),
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   start(now())
															#line 3194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this));

	}
	~ExtendTimeoutRepeatedlyActorState() 
	{
		fdb_probe_actor_destroy("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Version> __when_expr_0 = runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) { taskBucket->setOptions(tr); return map(tr->getReadVersion(), [=](Version v) { return v; }); });
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 1;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~ExtendTimeoutRepeatedlyActorState();
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 3239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Version const& __versionNow,int loopDepth) 
	{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		versionNow = __versionNow;
															#line 3248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Version && __versionNow,int loopDepth) 
	{
		versionNow = std::move(__versionNow);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		releaser = FlowLock::Releaser();
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0.8 * (BUGGIFY ? (2 * deterministicRandom()->random01()) : 1.0) * (double)(task->timeoutVersion - (uint64_t)versionNow) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 2;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (now() - start > 300)
															#line 3340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarnAlways, "TaskBucketLongExtend") .detail("Duration", now() - start) .detail("TaskUID", task->key) .detail("TaskType", task->params[Task::reservedTaskParamKeyType]) .detail("Priority", task->getPriority());
															#line 3344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = task->extendMutex.take();
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 3;
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (now() - start > 300)
															#line 3364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarnAlways, "TaskBucketLongExtend") .detail("Duration", now() - start) .detail("TaskUID", task->key) .detail("TaskType", task->params[Task::reservedTaskParamKeyType]) .detail("Priority", task->getPriority());
															#line 3368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = task->extendMutex.take();
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 3;
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		releaser = FlowLock::Releaser(task->extendMutex, 1);
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 3453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		releaser = FlowLock::Releaser(task->extendMutex, 1);
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 3464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1loopBody1cont4(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1(int loopDepth) 
	{
		try {
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->reset();
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Version> __when_expr_3 = taskBucket->extendTimeout(tr, task, UpdateParams::False);
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1loopBody1cont2loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1loopBody1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 4;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1loopBody1cont2loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2break1(int loopDepth) 
	{
		try {
			return a_body1cont1loopBody1cont4(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_5 = tr->onError(e);
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 3598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 2)); else return a_body1cont1loopBody1cont2loopBody1Catch1when1(__when_expr_5.get(), loopDepth); };
			static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 6;
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1cont2(int loopDepth) 
	{
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_4 = tr->commit();
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1loopBody1cont2loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont1loopBody1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 5;
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1when1(Version const& __newTimeout,int loopDepth) 
	{
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		newTimeout = __newTimeout;
															#line 3634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont2loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1when1(Version && __newTimeout,int loopDepth) 
	{
		newTimeout = std::move(__newTimeout);
		loopDepth = a_body1cont1loopBody1cont2loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont1loopBody1cont2loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = newTimeout;
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		versionNow = tr->getCommittedVersion();
															#line 3703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		return a_body1cont1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = newTimeout;
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		versionNow = tr->getCommittedVersion();
															#line 3714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		return a_body1cont1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont2loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont2loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont1loopBody1cont2loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont1loopBody1cont2loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont1loopBody1cont2loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	double start;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Version versionNow;
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	FlowLock::Releaser releaser;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Version newTimeout;
															#line 3873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via extendTimeoutRepeatedly()
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutRepeatedlyActor final : public Actor<Void>, public ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >, public FastAllocated<ExtendTimeoutRepeatedlyActor>, public ExtendTimeoutRepeatedlyActorState<ExtendTimeoutRepeatedlyActor> {
															#line 3878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<ExtendTimeoutRepeatedlyActor>::operator new;
	using FastAllocated<ExtendTimeoutRepeatedlyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >;
friend struct ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >;
friend struct ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >;
friend struct ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >;
friend struct ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >;
friend struct ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutRepeatedlyActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 3894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   ExtendTimeoutRepeatedlyActorState<ExtendTimeoutRepeatedlyActor>(cx, taskBucket, task)
	{
		fdb_probe_actor_enter("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("extendTimeoutRepeatedly");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("extendTimeoutRepeatedly", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> extendTimeoutRepeatedly( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<Task> const& task ) {
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new ExtendTimeoutRepeatedlyActor(cx, taskBucket, task));
															#line 3926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 3931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via doTask()
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DoTaskActor>
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoTaskActorState {
															#line 3937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoTaskActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFunc(),
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   verifyTask(false)
															#line 3954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("doTask", reinterpret_cast<unsigned long>(this));

	}
	~DoTaskActorState() 
	{
		fdb_probe_actor_destroy("doTask", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!task || !TaskFuncBase::isValidTask(task))
															#line 3969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(false); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 3973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(false);
				this->~DoTaskActorState();
				static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			try {
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				taskFunc = TaskFuncBase::create(task->params[Task::reservedTaskParamKeyType]);
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskFunc)
															#line 3984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					verifyTask.set(task->params.find(Task::reservedTaskParamValidKey) != task->params.end());
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					if (verifyTask)
															#line 3990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
					{
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
						;
															#line 3994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
						loopDepth = a_body1loopHead1(loopDepth);
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
		this->~DoTaskActorState();
		static_cast<DoTaskActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(true); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 4033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(true);
		this->~DoTaskActorState();
		static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarn, "TaskBucketExecuteFailure") .error(e) .detail("TaskUID", task->key) .detail("TaskType", task->params[Task::reservedTaskParamKeyType].printable()) .detail("Priority", task->getPriority());
															#line 4046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			try {
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				StrictFuture<Void> __when_expr_8 = taskFunc->handleError(cx, task, e);
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2Catch1(actor_cancelled(), loopDepth);
															#line 4052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch2Catch1(__when_expr_8.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_8.get(), loopDepth); };
				static_cast<DoTaskActor*>(this)->actor_wait_state = 9;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 8, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2Catch1(unknown_error(), loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(int loopDepth) 
	{
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_5 = taskFunc->execute(cx, taskBucket, futureBucket, task) || extendTimeoutRepeatedly(cx, taskBucket, task);
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch2(__when_expr_5.getError(), loopDepth); else return a_body1cont4when1(__when_expr_5.get(), loopDepth); };
		static_cast<DoTaskActor*>(this)->actor_wait_state = 6;
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 5, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

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
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskBucket->setOptions(tr);
															#line 4115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		try {
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = taskVerify(taskBucket, tr, task);
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 1;
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 0, bool >*>(static_cast<DoTaskActor*>(this)));
															#line 4126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
			return a_body1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 5;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 4, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& validTask,int loopDepth) 
	{
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validTask)
															#line 4183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskBucket->isFinished(tr, task);
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 2;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 1, bool >*>(static_cast<DoTaskActor*>(this)));
															#line 4194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && validTask,int loopDepth) 
	{
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validTask)
															#line 4208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskBucket->isFinished(tr, task);
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 2;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 1, bool >*>(static_cast<DoTaskActor*>(this)));
															#line 4219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& validTask,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(validTask, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && validTask,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(validTask), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4(bool const& isFinished,int loopDepth) 
	{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!isFinished)
															#line 4302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_2 = taskBucket->finish(tr, task);
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 3;
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 2, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4(bool && isFinished,int loopDepth) 
	{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!isFinished)
															#line 4327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_2 = taskBucket->finish(tr, task);
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 3;
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 2, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(bool const& isFinished,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(isFinished, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(bool && isFinished,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(isFinished), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<DoTaskActor*>(this)->actor_wait_state = 4;
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 3, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(true); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 4506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(true);
		this->~DoTaskActorState();
		static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(true); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 4518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(true);
		this->~DoTaskActorState();
		static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
	void a_exitChoose4() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 3);

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
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (BUGGIFY)
															#line 4668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delay(10.0);
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch2(__when_expr_6.getError(), loopDepth); else return a_body1cont6when1(__when_expr_6.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 7;
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 6, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (BUGGIFY)
															#line 4693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delay(10.0);
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch2(__when_expr_6.getError(), loopDepth); else return a_body1cont6when1(__when_expr_6.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 7;
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 6, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

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
	void a_exitChoose6() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont7(int loopDepth) 
	{
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_7 = runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) { return finishTaskRun(tr, taskBucket, futureBucket, task, taskFunc, verifyTask); });
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch2(__when_expr_7.getError(), loopDepth); else return a_body1cont7when1(__when_expr_7.get(), loopDepth); };
		static_cast<DoTaskActor*>(this)->actor_wait_state = 8;
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 7, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1cont10(int loopDepth) 
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
	int a_body1Catch2cont1(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarn, "TaskBucketExecuteFailureLogErrorFailed") .error(e) .detail("TaskUID", task->key.printable()) .detail("TaskType", task->params[Task::reservedTaskParamKeyType].printable()) .detail("Priority", task->getPriority());
															#line 4967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = a_body1Catch2cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont3(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont3(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose9() 
	{
		if (static_cast<DoTaskActor*>(this)->actor_wait_state > 0) static_cast<DoTaskActor*>(this)->actor_wait_state = 0;
		static_cast<DoTaskActor*>(this)->ActorCallback< DoTaskActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2Catch1(error, 0);
		} catch (...) {
			a_body1Catch2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< DoTaskActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2Catch1(error, 0);
		} catch (...) {
			a_body1Catch2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< DoTaskActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1Catch2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2Catch1(error, 0);
		} catch (...) {
			a_body1Catch2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), 8);

	}
	int a_body1Catch2cont3(int loopDepth) 
	{
		try {
			loopDepth = a_body1Catch2cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuncBase> taskFunc;
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	VerifyTask verifyTask;
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 5080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via doTask()
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoTaskActor final : public Actor<bool>, public ActorCallback< DoTaskActor, 0, bool >, public ActorCallback< DoTaskActor, 1, bool >, public ActorCallback< DoTaskActor, 2, Void >, public ActorCallback< DoTaskActor, 3, Void >, public ActorCallback< DoTaskActor, 4, Void >, public ActorCallback< DoTaskActor, 5, Void >, public ActorCallback< DoTaskActor, 6, Void >, public ActorCallback< DoTaskActor, 7, Void >, public ActorCallback< DoTaskActor, 8, Void >, public FastAllocated<DoTaskActor>, public DoTaskActorState<DoTaskActor> {
															#line 5085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DoTaskActor>::operator new;
	using FastAllocated<DoTaskActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoTaskActor, 0, bool >;
friend struct ActorCallback< DoTaskActor, 1, bool >;
friend struct ActorCallback< DoTaskActor, 2, Void >;
friend struct ActorCallback< DoTaskActor, 3, Void >;
friend struct ActorCallback< DoTaskActor, 4, Void >;
friend struct ActorCallback< DoTaskActor, 5, Void >;
friend struct ActorCallback< DoTaskActor, 6, Void >;
friend struct ActorCallback< DoTaskActor, 7, Void >;
friend struct ActorCallback< DoTaskActor, 8, Void >;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoTaskActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 5104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   DoTaskActorState<DoTaskActor>(cx, taskBucket, futureBucket, task)
	{
		fdb_probe_actor_enter("doTask", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doTask");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doTask", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoTaskActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoTaskActor, 1, bool >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DoTaskActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DoTaskActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< DoTaskActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< DoTaskActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< DoTaskActor, 6, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< DoTaskActor, 7, Void >*)0, actor_cancelled()); break;
		case 9: this->a_callback_error((ActorCallback< DoTaskActor, 8, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> doTask( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, Reference<Task> const& task ) {
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new DoTaskActor(cx, taskBucket, futureBucket, task));
															#line 5139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 5144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via dispatch()
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DispatchActor>
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DispatchActorState {
															#line 5150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DispatchActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   pollDelay(pollDelay),
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   maxConcurrentTasks(maxConcurrentTasks),
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tasks(maxConcurrentTasks)
															#line 5167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("dispatch", reinterpret_cast<unsigned long>(this));

	}
	~DispatchActorState() 
	{
		fdb_probe_actor_destroy("dispatch", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& f : tasks ) {
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				f = Never();
															#line 5184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSlots = std::vector<int>();
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSlots.reserve(tasks.size());
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int i = 0;i < tasks.size();++i) {
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.push_back(i);
															#line 5194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getTasks = std::vector<Future<Reference<Task>>>();
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = 1;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 5202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~DispatchActorState();
		static_cast<DispatchActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++taskBucket->dispatchSlotChecksStarted;
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 5234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++taskBucket->dispatchSlotChecksComplete;
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Future<Void> w = ready(waitForAny(tasks));
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!availableSlots.empty())
															#line 5247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (*pollDelay > 600)
															#line 5251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				TraceEvent(SevWarnAlways, "TaskBucketLongPollDelay").suppressFor(1.0).detail("Delay", *pollDelay);
															#line 5255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			w = w || delay(*pollDelay * (0.9 + deterministicRandom()->random01() / 5));
															#line 5259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = w;
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DispatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DispatchActor*>(this)->actor_wait_state = 2;
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DispatchActor, 1, Void >*>(static_cast<DispatchActor*>(this)));
															#line 5270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(!availableSlots.empty()))
															#line 5286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		getTasks.clear();
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0, imax = std::min<unsigned int>(getBatchSize, availableSlots.size());i < imax;++i) {
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getTasks.push_back(taskBucket->getOne(cx));
															#line 5296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_0 = waitForAllReady(getTasks);
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DispatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 5302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DispatchActor*>(this)->actor_wait_state = 1;
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DispatchActor, 0, Void >*>(static_cast<DispatchActor*>(this)));
															#line 5307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		bool done = false;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < getTasks.size();++i) {
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (getTasks[i].isError())
															#line 5333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchErrors;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				continue;
			}
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Reference<Task> task = getTasks[i].get();
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task)
															#line 5346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchDoTasks;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				int slot = availableSlots.back();
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.pop_back();
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[slot] = taskBucket->doTask(cx, futureBucket, task);
															#line 5356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			else
			{
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchEmptyTasks;
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (done)
															#line 5369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = 1;
															#line 5373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		else
		{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = std::min<unsigned int>(getBatchSize * 2, maxConcurrentTasks);
															#line 5380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		bool done = false;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < getTasks.size();++i) {
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (getTasks[i].isError())
															#line 5394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchErrors;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				continue;
			}
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Reference<Task> task = getTasks[i].get();
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task)
															#line 5407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchDoTasks;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				int slot = availableSlots.back();
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.pop_back();
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[slot] = taskBucket->doTask(cx, futureBucket, task);
															#line 5417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			else
			{
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchEmptyTasks;
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (done)
															#line 5430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = 1;
															#line 5434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		else
		{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = std::min<unsigned int>(getBatchSize * 2, maxConcurrentTasks);
															#line 5441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
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
		if (static_cast<DispatchActor*>(this)->actor_wait_state > 0) static_cast<DispatchActor*>(this)->actor_wait_state = 0;
		static_cast<DispatchActor*>(this)->ActorCallback< DispatchActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DispatchActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DispatchActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DispatchActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < tasks.size();++i) {
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (tasks[i].isReady())
															#line 5516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.push_back(i);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[i] = Never();
															#line 5522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < tasks.size();++i) {
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (tasks[i].isReady())
															#line 5535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.push_back(i);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[i] = Never();
															#line 5541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
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
		if (static_cast<DispatchActor*>(this)->actor_wait_state > 0) static_cast<DispatchActor*>(this)->actor_wait_state = 0;
		static_cast<DispatchActor*>(this)->ActorCallback< DispatchActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DispatchActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DispatchActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DispatchActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::shared_ptr<double const> pollDelay;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int maxConcurrentTasks;
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<bool>> tasks;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<int> availableSlots;
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<Reference<Task>>> getTasks;
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	unsigned int getBatchSize;
															#line 5629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via dispatch()
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DispatchActor final : public Actor<Void>, public ActorCallback< DispatchActor, 0, Void >, public ActorCallback< DispatchActor, 1, Void >, public FastAllocated<DispatchActor>, public DispatchActorState<DispatchActor> {
															#line 5634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DispatchActor>::operator new;
	using FastAllocated<DispatchActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DispatchActor, 0, Void >;
friend struct ActorCallback< DispatchActor, 1, Void >;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DispatchActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 5646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   DispatchActorState<DispatchActor>(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks)
	{
		fdb_probe_actor_enter("dispatch", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("dispatch");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("dispatch", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DispatchActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DispatchActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> dispatch( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, std::shared_ptr<double const> const& pollDelay, int const& maxConcurrentTasks ) {
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new DispatchActor(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks));
															#line 5674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 5679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via watchPaused()
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class WatchPausedActor>
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class WatchPausedActorState {
															#line 5685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	WatchPausedActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<AsyncVar<bool>> const& paused) 
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   paused(paused)
															#line 5696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("watchPaused", reinterpret_cast<unsigned long>(this));

	}
	~WatchPausedActorState() 
	{
		fdb_probe_actor_destroy("watchPaused", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 5711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~WatchPausedActorState();
		static_cast<WatchPausedActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 5741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		try {
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(taskBucket->pauseKey);
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WatchPausedActor*>(this)->actor_wait_state = 1;
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 0, Optional<Value> >*>(static_cast<WatchPausedActor*>(this)));
															#line 5754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr->onError(e);
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<WatchPausedActor*>(this)->actor_wait_state = 4;
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 3, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& pausedVal,int loopDepth) 
	{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		paused->set(pausedVal.present());
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		watchPausedFuture = tr->watch(taskBucket->pauseKey);
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 2;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 1, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && pausedVal,int loopDepth) 
	{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		paused->set(pausedVal.present());
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		watchPausedFuture = tr->watch(taskBucket->pauseKey);
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 2;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 1, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& pausedVal,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(pausedVal, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && pausedVal,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(pausedVal), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state > 0) static_cast<WatchPausedActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPausedActor*>(this)->ActorCallback< WatchPausedActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchPausedActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = watchPausedFuture;
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 3;
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 2, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = watchPausedFuture;
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 3;
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 2, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state > 0) static_cast<WatchPausedActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPausedActor*>(this)->ActorCallback< WatchPausedActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WatchPausedActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state > 0) static_cast<WatchPausedActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPausedActor*>(this)->ActorCallback< WatchPausedActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WatchPausedActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state > 0) static_cast<WatchPausedActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPausedActor*>(this)->ActorCallback< WatchPausedActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WatchPausedActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WatchPausedActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<AsyncVar<bool>> paused;
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Future<Void> watchPausedFuture;
															#line 6165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via watchPaused()
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class WatchPausedActor final : public Actor<Void>, public ActorCallback< WatchPausedActor, 0, Optional<Value> >, public ActorCallback< WatchPausedActor, 1, Void >, public ActorCallback< WatchPausedActor, 2, Void >, public ActorCallback< WatchPausedActor, 3, Void >, public FastAllocated<WatchPausedActor>, public WatchPausedActorState<WatchPausedActor> {
															#line 6170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<WatchPausedActor>::operator new;
	using FastAllocated<WatchPausedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchPausedActor, 0, Optional<Value> >;
friend struct ActorCallback< WatchPausedActor, 1, Void >;
friend struct ActorCallback< WatchPausedActor, 2, Void >;
friend struct ActorCallback< WatchPausedActor, 3, Void >;
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	WatchPausedActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<AsyncVar<bool>> const& paused) 
															#line 6184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   WatchPausedActorState<WatchPausedActor>(cx, taskBucket, paused)
	{
		fdb_probe_actor_enter("watchPaused", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("watchPaused");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("watchPaused", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchPausedActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WatchPausedActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WatchPausedActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WatchPausedActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> watchPaused( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<AsyncVar<bool>> const& paused ) {
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new WatchPausedActor(cx, taskBucket, paused));
															#line 6214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 6219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via run()
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class RunActor>
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RunActorState {
															#line 6225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RunActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   pollDelay(pollDelay),
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   maxConcurrentTasks(maxConcurrentTasks),
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   paused(makeReference<AsyncVar<bool>>(true)),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   watchPausedFuture(watchPaused(cx, taskBucket, paused))
															#line 6244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("run", reinterpret_cast<unsigned long>(this));

	}
	~RunActorState() 
	{
		fdb_probe_actor_destroy("run", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->metricLogger = traceCounters( "TaskBucketMetrics", taskBucket->dbgid, CLIENT_KNOBS->TASKBUCKET_LOGGING_DELAY, &taskBucket->cc);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 6261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~RunActorState();
		static_cast<RunActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 6291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = dispatch(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks) || paused->onChange() || watchPausedFuture;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunActor*>(this)->actor_wait_state = 2;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunActor, 1, Void >*>(static_cast<RunActor*>(this)));
															#line 6307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(paused->get()))
															#line 6323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_0 = paused->onChange() || watchPausedFuture;
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 6331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RunActor*>(this)->actor_wait_state = 1;
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunActor, 0, Void >*>(static_cast<RunActor*>(this)));
															#line 6336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<RunActor*>(this)->actor_wait_state > 0) static_cast<RunActor*>(this)->actor_wait_state = 0;
		static_cast<RunActor*>(this)->ActorCallback< RunActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
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
		if (static_cast<RunActor*>(this)->actor_wait_state > 0) static_cast<RunActor*>(this)->actor_wait_state = 0;
		static_cast<RunActor*>(this)->ActorCallback< RunActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::shared_ptr<double const> pollDelay;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int maxConcurrentTasks;
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<AsyncVar<bool>> paused;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Future<Void> watchPausedFuture;
															#line 6518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via run()
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RunActor final : public Actor<Void>, public ActorCallback< RunActor, 0, Void >, public ActorCallback< RunActor, 1, Void >, public FastAllocated<RunActor>, public RunActorState<RunActor> {
															#line 6523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<RunActor>::operator new;
	using FastAllocated<RunActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunActor, 0, Void >;
friend struct ActorCallback< RunActor, 1, Void >;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RunActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 6535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   RunActorState<RunActor>(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks)
	{
		fdb_probe_actor_enter("run", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("run");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("run", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> run( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, std::shared_ptr<double const> const& pollDelay, int const& maxConcurrentTasks ) {
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new RunActor(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks));
															#line 6563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	static Future<Standalone<StringRef>> addIdle(Reference<ReadYourWritesTransaction> tr,
	                                             Reference<TaskBucket> taskBucket) {
		taskBucket->setOptions(tr);

		auto newTask = makeReference<Task>(IdleTaskFunc::name, IdleTaskFunc::version);
		return taskBucket->addTask(tr, newTask);
	}

	static Future<Standalone<StringRef>> addIdle(Database cx, Reference<TaskBucket> taskBucket) {
		return runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) { return addIdle(tr, taskBucket); });
	}

																#line 6580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isEmpty()
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsEmptyActor>
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActorState {
															#line 6586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 6595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("isEmpty", reinterpret_cast<unsigned long>(this));

	}
	~IsEmptyActorState() 
	{
		fdb_probe_actor_destroy("isEmpty", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			resultFutures = std::vector<Future<RangeResult>>();
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int pri = 0;pri <= CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;++pri) {
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				resultFutures.push_back(tr->getRange(taskBucket->getAvailableSpace(pri).range(), 1));
															#line 6616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = int();
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = 0;
															#line 6622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~IsEmptyActorState();
		static_cast<IsEmptyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(taskBucket->timeouts.range(), 1);
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<IsEmptyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<IsEmptyActor*>(this)->actor_wait_state = 2;
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IsEmptyActor, 1, RangeResult >*>(static_cast<IsEmptyActor*>(this)));
															#line 6652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(i < resultFutures.size()))
															#line 6668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = resultFutures[i];
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<IsEmptyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<IsEmptyActor*>(this)->actor_wait_state = 1;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsEmptyActor, 0, RangeResult >*>(static_cast<IsEmptyActor*>(this)));
															#line 6681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
	int a_body1loopBody1cont1(RangeResult const& results,int loopDepth) 
	{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 6703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 6715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && results,int loopDepth) 
	{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 6724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 6736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& results,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(results, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && results,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IsEmptyActor*>(this)->actor_wait_state > 0) static_cast<IsEmptyActor*>(this)->actor_wait_state = 0;
		static_cast<IsEmptyActor*>(this)->ActorCallback< IsEmptyActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< IsEmptyActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IsEmptyActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IsEmptyActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(RangeResult const& values,int loopDepth) 
	{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 6808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsEmptyActorState();
		static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(RangeResult && values,int loopDepth) 
	{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 6832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsEmptyActorState();
		static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& values,int loopDepth) 
	{
		loopDepth = a_body1cont3(values, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && values,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(values), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IsEmptyActor*>(this)->actor_wait_state > 0) static_cast<IsEmptyActor*>(this)->actor_wait_state = 0;
		static_cast<IsEmptyActor*>(this)->ActorCallback< IsEmptyActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< IsEmptyActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IsEmptyActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IsEmptyActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<RangeResult>> resultFutures;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int i;
															#line 6923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isEmpty()
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActor final : public Actor<bool>, public ActorCallback< IsEmptyActor, 0, RangeResult >, public ActorCallback< IsEmptyActor, 1, RangeResult >, public FastAllocated<IsEmptyActor>, public IsEmptyActorState<IsEmptyActor> {
															#line 6928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsEmptyActor>::operator new;
	using FastAllocated<IsEmptyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsEmptyActor, 0, RangeResult >;
friend struct ActorCallback< IsEmptyActor, 1, RangeResult >;
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 6940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   IsEmptyActorState<IsEmptyActor>(tr, taskBucket)
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("isEmpty");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IsEmptyActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IsEmptyActor, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isEmpty( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsEmptyActor(tr, taskBucket));
															#line 6968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 6973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isBusy()
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsBusyActor>
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsBusyActorState {
															#line 6979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsBusyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 6988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("isBusy", reinterpret_cast<unsigned long>(this));

	}
	~IsBusyActorState() 
	{
		fdb_probe_actor_destroy("isBusy", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			resultFutures = std::vector<Future<RangeResult>>();
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int pri = 0;pri <= CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;++pri) {
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				resultFutures.push_back(tr->getRange(taskBucket->getAvailableSpace(pri).range(), 1));
															#line 7009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = int();
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = 0;
															#line 7015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~IsBusyActorState();
		static_cast<IsBusyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsBusyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsBusyActorState(); static_cast<IsBusyActor*>(this)->destroy(); return 0; }
															#line 7038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsBusyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~IsBusyActorState();
		static_cast<IsBusyActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(i < resultFutures.size()))
															#line 7057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = resultFutures[i];
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<IsBusyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<IsBusyActor*>(this)->actor_wait_state = 1;
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsBusyActor, 0, RangeResult >*>(static_cast<IsBusyActor*>(this)));
															#line 7070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
	int a_body1loopBody1cont1(RangeResult const& results,int loopDepth) 
	{
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 7092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsBusyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsBusyActorState(); static_cast<IsBusyActor*>(this)->destroy(); return 0; }
															#line 7096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsBusyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~IsBusyActorState();
			static_cast<IsBusyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 7104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && results,int loopDepth) 
	{
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 7113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsBusyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsBusyActorState(); static_cast<IsBusyActor*>(this)->destroy(); return 0; }
															#line 7117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsBusyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~IsBusyActorState();
			static_cast<IsBusyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 7125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& results,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(results, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && results,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IsBusyActor*>(this)->actor_wait_state > 0) static_cast<IsBusyActor*>(this)->actor_wait_state = 0;
		static_cast<IsBusyActor*>(this)->ActorCallback< IsBusyActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< IsBusyActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("isBusy", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isBusy", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IsBusyActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("isBusy", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isBusy", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IsBusyActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("isBusy", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isBusy", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<RangeResult>> resultFutures;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int i;
															#line 7201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isBusy()
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsBusyActor final : public Actor<bool>, public ActorCallback< IsBusyActor, 0, RangeResult >, public FastAllocated<IsBusyActor>, public IsBusyActorState<IsBusyActor> {
															#line 7206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsBusyActor>::operator new;
	using FastAllocated<IsBusyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsBusyActor, 0, RangeResult >;
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsBusyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 7217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   IsBusyActorState<IsBusyActor>(tr, taskBucket)
	{
		fdb_probe_actor_enter("isBusy", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("isBusy");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("isBusy", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IsBusyActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isBusy( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsBusyActor(tr, taskBucket));
															#line 7244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	// Verify that the task's keys are still in the timeout space at the expected timeout prefix
																#line 7250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isFinished()
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsFinishedActor>
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsFinishedActorState {
															#line 7256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsFinishedActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 7267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("isFinished", reinterpret_cast<unsigned long>(this));

	}
	~IsFinishedActorState() 
	{
		fdb_probe_actor_destroy("isFinished", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple t;
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			t.append(task->timeoutVersion);
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			t.append(task->key);
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(taskBucket->timeouts.range(t), 1);
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<IsFinishedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsFinishedActor*>(this)->actor_wait_state = 1;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsFinishedActor, 0, RangeResult >*>(static_cast<IsFinishedActor*>(this)));
															#line 7297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~IsFinishedActorState();
		static_cast<IsFinishedActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& values,int loopDepth) 
	{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 7320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsFinishedActorState();
			static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsFinishedActorState();
		static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 7344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsFinishedActorState();
			static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsFinishedActorState();
		static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& values,int loopDepth) 
	{
		loopDepth = a_body1cont1(values, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && values,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(values), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IsFinishedActor*>(this)->actor_wait_state > 0) static_cast<IsFinishedActor*>(this)->actor_wait_state = 0;
		static_cast<IsFinishedActor*>(this)->ActorCallback< IsFinishedActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< IsFinishedActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("isFinished", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isFinished", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IsFinishedActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("isFinished", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isFinished", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IsFinishedActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("isFinished", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isFinished", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 7433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isFinished()
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsFinishedActor final : public Actor<bool>, public ActorCallback< IsFinishedActor, 0, RangeResult >, public FastAllocated<IsFinishedActor>, public IsFinishedActorState<IsFinishedActor> {
															#line 7438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsFinishedActor>::operator new;
	using FastAllocated<IsFinishedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsFinishedActor, 0, RangeResult >;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsFinishedActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 7449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   IsFinishedActorState<IsFinishedActor>(tr, taskBucket, task)
	{
		fdb_probe_actor_enter("isFinished", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("isFinished");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("isFinished", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IsFinishedActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isFinished( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<Task> const& task ) {
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsFinishedActor(tr, taskBucket, task));
															#line 7476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 7481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getActiveKey()
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetActiveKeyActor>
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetActiveKeyActorState {
															#line 7487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetActiveKeyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Optional<Value> const& startingValue) 
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   startingValue(startingValue)
															#line 7498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("getActiveKey", reinterpret_cast<unsigned long>(this));

	}
	~GetActiveKeyActorState() 
	{
		fdb_probe_actor_destroy("getActiveKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(taskBucket->active.key());
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetActiveKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetActiveKeyActor*>(this)->actor_wait_state = 1;
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetActiveKeyActor, 0, Optional<Value> >*>(static_cast<GetActiveKeyActor*>(this)));
															#line 7522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~GetActiveKeyActorState();
		static_cast<GetActiveKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& new_value,int loopDepth) 
	{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (new_value != startingValue)
															#line 7545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~GetActiveKeyActorState();
			static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~GetActiveKeyActorState();
		static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && new_value,int loopDepth) 
	{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (new_value != startingValue)
															#line 7569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~GetActiveKeyActorState();
			static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~GetActiveKeyActorState();
		static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& new_value,int loopDepth) 
	{
		loopDepth = a_body1cont1(new_value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && new_value,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(new_value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetActiveKeyActor*>(this)->actor_wait_state > 0) static_cast<GetActiveKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetActiveKeyActor*>(this)->ActorCallback< GetActiveKeyActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetActiveKeyActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getActiveKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getActiveKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetActiveKeyActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getActiveKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getActiveKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetActiveKeyActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getActiveKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getActiveKey", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Optional<Value> startingValue;
															#line 7658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getActiveKey()
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetActiveKeyActor final : public Actor<bool>, public ActorCallback< GetActiveKeyActor, 0, Optional<Value> >, public FastAllocated<GetActiveKeyActor>, public GetActiveKeyActorState<GetActiveKeyActor> {
															#line 7663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetActiveKeyActor>::operator new;
	using FastAllocated<GetActiveKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetActiveKeyActor, 0, Optional<Value> >;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetActiveKeyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Optional<Value> const& startingValue) 
															#line 7674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   GetActiveKeyActorState<GetActiveKeyActor>(tr, taskBucket, startingValue)
	{
		fdb_probe_actor_enter("getActiveKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getActiveKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getActiveKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetActiveKeyActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> getActiveKey( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Optional<Value> const& startingValue ) {
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new GetActiveKeyActor(tr, taskBucket, startingValue));
															#line 7701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 7706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via checkActive()
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class CheckActiveActor>
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class CheckActiveActorState {
															#line 7712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	CheckActiveActorState(Database const& cx,Reference<TaskBucket> const& taskBucket) 
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx)),
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   startingValue()
															#line 7725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("checkActive", reinterpret_cast<unsigned long>(this));

	}
	~CheckActiveActorState() 
	{
		fdb_probe_actor_destroy("checkActive", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 7740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~CheckActiveActorState();
		static_cast<CheckActiveActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		idx = 0;
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 7765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

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
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = isBusy(tr, taskBucket);
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 1;
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 0, bool >*>(static_cast<CheckActiveActor*>(this)));
															#line 7791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 5;
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 4, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 7833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& is_busy,int loopDepth) 
	{
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!is_busy)
															#line 7848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(addIdle(tr, taskBucket));
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 2;
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 1, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 7859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && is_busy,int loopDepth) 
	{
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!is_busy)
															#line 7873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(addIdle(tr, taskBucket));
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 2;
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 1, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 7884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& is_busy,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(is_busy, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && is_busy,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(is_busy), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Optional<Value>> __when_expr_2 = tr->get(taskBucket->active.key());
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 3;
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 2, Optional<Value> >*>(static_cast<CheckActiveActor*>(this)));
															#line 7968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(Optional<Value> const& val,int loopDepth) 
	{
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		startingValue = val;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 4;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 3, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5(Optional<Value> && val,int loopDepth) 
	{
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		startingValue = val;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 4;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 3, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(val, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 2, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 2, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 2, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 2, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 3);

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
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TEST(true);
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<CheckActiveActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckActiveActorState(); static_cast<CheckActiveActor*>(this)->destroy(); return 0; }
															#line 8303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<CheckActiveActor*>(this)->SAV< bool >::value()) bool(false);
		this->~CheckActiveActorState();
		static_cast<CheckActiveActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(idx < CLIENT_KNOBS->TASKBUCKET_CHECK_ACTIVE_AMOUNT))
															#line 8322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 8330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++idx;
															#line 8352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1(int loopDepth) 
	{
		try {
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_5 = delay(CLIENT_KNOBS->TASKBUCKET_CHECK_ACTIVE_DELAY);
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1cont1loopBody1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont1loopBody1loopBody1when1(__when_expr_5.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 6;
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 5, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1loopBody1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1loopBody1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1cont1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_7 = tr->onError(e);
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 8415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), std::max(0, loopDepth - 2)); else return a_body1cont1loopBody1loopBody1Catch1when1(__when_expr_7.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 8;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 7, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<bool> __when_expr_6 = getActiveKey(tr, taskBucket, startingValue);
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1cont1loopBody1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont1loopBody1loopBody1cont2when1(__when_expr_6.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 7;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 6, bool >*>(static_cast<CheckActiveActor*>(this)));
															#line 8442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<bool> __when_expr_6 = getActiveKey(tr, taskBucket, startingValue);
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1cont1loopBody1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont1loopBody1loopBody1cont2when1(__when_expr_6.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 7;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 6, bool >*>(static_cast<CheckActiveActor*>(this)));
															#line 8458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont1loopBody1loopBody1cont3(bool const& isActiveKey,int loopDepth) 
	{
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isActiveKey)
															#line 8530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<CheckActiveActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckActiveActorState(); static_cast<CheckActiveActor*>(this)->destroy(); return 0; }
															#line 8536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<CheckActiveActor*>(this)->SAV< bool >::value()) bool(true);
			this->~CheckActiveActorState();
			static_cast<CheckActiveActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		return a_body1cont1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont3(bool && isActiveKey,int loopDepth) 
	{
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isActiveKey)
															#line 8550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<CheckActiveActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckActiveActorState(); static_cast<CheckActiveActor*>(this)->destroy(); return 0; }
															#line 8556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<CheckActiveActor*>(this)->SAV< bool >::value()) bool(true);
			this->~CheckActiveActorState();
			static_cast<CheckActiveActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		return a_body1cont1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont2when1(bool const& isActiveKey,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1cont3(isActiveKey, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont2when1(bool && isActiveKey,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1cont3(std::move(isActiveKey), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 6, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 6, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont1loopBody1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 6, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont1loopBody1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 6, bool >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont1loopBody1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state > 0) static_cast<CheckActiveActor*>(this)->actor_wait_state = 0;
		static_cast<CheckActiveActor*>(this)->ActorCallback< CheckActiveActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont1loopBody1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< CheckActiveActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont1loopBody1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< CheckActiveActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), 7);

	}
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Optional<Value> startingValue;
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int idx;
															#line 8714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via checkActive()
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class CheckActiveActor final : public Actor<bool>, public ActorCallback< CheckActiveActor, 0, bool >, public ActorCallback< CheckActiveActor, 1, Void >, public ActorCallback< CheckActiveActor, 2, Optional<Value> >, public ActorCallback< CheckActiveActor, 3, Void >, public ActorCallback< CheckActiveActor, 4, Void >, public ActorCallback< CheckActiveActor, 5, Void >, public ActorCallback< CheckActiveActor, 6, bool >, public ActorCallback< CheckActiveActor, 7, Void >, public FastAllocated<CheckActiveActor>, public CheckActiveActorState<CheckActiveActor> {
															#line 8719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<CheckActiveActor>::operator new;
	using FastAllocated<CheckActiveActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckActiveActor, 0, bool >;
friend struct ActorCallback< CheckActiveActor, 1, Void >;
friend struct ActorCallback< CheckActiveActor, 2, Optional<Value> >;
friend struct ActorCallback< CheckActiveActor, 3, Void >;
friend struct ActorCallback< CheckActiveActor, 4, Void >;
friend struct ActorCallback< CheckActiveActor, 5, Void >;
friend struct ActorCallback< CheckActiveActor, 6, bool >;
friend struct ActorCallback< CheckActiveActor, 7, Void >;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	CheckActiveActor(Database const& cx,Reference<TaskBucket> const& taskBucket) 
															#line 8737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   CheckActiveActorState<CheckActiveActor>(cx, taskBucket)
	{
		fdb_probe_actor_enter("checkActive", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkActive");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkActive", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckActiveActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckActiveActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CheckActiveActor, 2, Optional<Value> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CheckActiveActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< CheckActiveActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< CheckActiveActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< CheckActiveActor, 6, bool >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< CheckActiveActor, 7, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> checkActive( Database const& cx, Reference<TaskBucket> const& taskBucket ) {
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new CheckActiveActor(cx, taskBucket));
															#line 8771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 8776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getTaskCount()
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetTaskCountActor>
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskCountActorState {
															#line 8782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskCountActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 8791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("getTaskCount", reinterpret_cast<unsigned long>(this));

	}
	~GetTaskCountActorState() 
	{
		fdb_probe_actor_destroy("getTaskCount", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(taskBucket->prefix.pack(LiteralStringRef("task_count")));
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetTaskCountActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTaskCountActor*>(this)->actor_wait_state = 1;
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTaskCountActor, 0, Optional<Value> >*>(static_cast<GetTaskCountActor*>(this)));
															#line 8815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~GetTaskCountActorState();
		static_cast<GetTaskCountActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!val.present())
															#line 8838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(0); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(0);
			this->~GetTaskCountActorState();
			static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(val.get().size() == sizeof(int64_t));
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		int64_t intValue = 0;
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		memcpy(&intValue, val.get().begin(), val.get().size());
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(intValue); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(intValue);
		this->~GetTaskCountActorState();
		static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!val.present())
															#line 8868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(0); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(0);
			this->~GetTaskCountActorState();
			static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(val.get().size() == sizeof(int64_t));
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		int64_t intValue = 0;
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		memcpy(&intValue, val.get().begin(), val.get().size());
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(intValue); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(intValue);
		this->~GetTaskCountActorState();
		static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTaskCountActor*>(this)->actor_wait_state > 0) static_cast<GetTaskCountActor*>(this)->actor_wait_state = 0;
		static_cast<GetTaskCountActor*>(this)->ActorCallback< GetTaskCountActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTaskCountActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getTaskCount", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskCount", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTaskCountActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getTaskCount", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskCount", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTaskCountActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getTaskCount", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTaskCount", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 8961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getTaskCount()
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskCountActor final : public Actor<int64_t>, public ActorCallback< GetTaskCountActor, 0, Optional<Value> >, public FastAllocated<GetTaskCountActor>, public GetTaskCountActorState<GetTaskCountActor> {
															#line 8966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetTaskCountActor>::operator new;
	using FastAllocated<GetTaskCountActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTaskCountActor, 0, Optional<Value> >;
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskCountActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 8977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<int64_t>(),
		   GetTaskCountActorState<GetTaskCountActor>(tr, taskBucket)
	{
		fdb_probe_actor_enter("getTaskCount", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTaskCount");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTaskCount", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTaskCountActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<int64_t> getTaskCount( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<int64_t>(new GetTaskCountActor(tr, taskBucket));
															#line 9004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	// Looks for tasks that have timed out and returns them to be available tasks.
	// Returns True if any tasks were affected.
																#line 9011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via requeueTimedOutTasks()
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class RequeueTimedOutTasksActor>
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RequeueTimedOutTasksActorState {
															#line 9017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RequeueTimedOutTasksActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 9026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this));

	}
	~RequeueTimedOutTasksActorState() 
	{
		fdb_probe_actor_destroy("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Version> __when_expr_0 = tr->getReadVersion();
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 1;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RequeueTimedOutTasksActor, 0, Version >*>(static_cast<RequeueTimedOutTasksActor*>(this)));
															#line 9050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~RequeueTimedOutTasksActorState();
		static_cast<RequeueTimedOutTasksActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Version const& end,int loopDepth) 
	{
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		range = KeyRange(KeyRangeRef(taskBucket->timeouts.get(0).range().begin, taskBucket->timeouts.get(end).range().end));
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(range, CLIENT_KNOBS->TASKBUCKET_MAX_TASK_KEYS);
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 2;
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*>(static_cast<RequeueTimedOutTasksActor*>(this)));
															#line 9082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Version && end,int loopDepth) 
	{
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		range = KeyRange(KeyRangeRef(taskBucket->timeouts.get(0).range().begin, taskBucket->timeouts.get(end).range().end));
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(range, CLIENT_KNOBS->TASKBUCKET_MAX_TASK_KEYS);
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 2;
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*>(static_cast<RequeueTimedOutTasksActor*>(this)));
															#line 9100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Version const& end,int loopDepth) 
	{
		loopDepth = a_body1cont1(end, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Version && end,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(end), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state > 0) static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 0;
		static_cast<RequeueTimedOutTasksActor*>(this)->ActorCallback< RequeueTimedOutTasksActor, 0, Version >::remove();

	}
	void a_callback_fire(ActorCallback< RequeueTimedOutTasksActor, 0, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RequeueTimedOutTasksActor, 0, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RequeueTimedOutTasksActor, 0, Version >*,Error err) 
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(RangeResult const& values,int loopDepth) 
	{
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Task task;
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Key lastKey;
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& iter : values ) {
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple t = taskBucket->timeouts.unpack(iter.key);
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key uid = t.getString(1);
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = t.getString(2);
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (uid != task.key)
															#line 9184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				for( auto& p : task.params ) {
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					tr->set(space.pack(p.key), p.value);
															#line 9192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.params.clear();
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.key = uid;
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastKey = iter.key;
															#line 9200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task.params[param] = iter.value;
															#line 9204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!values.more)
															#line 9208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task.params ) {
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(space.pack(p.key), p.value);
															#line 9216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (values.size() > 0)
															#line 9220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->clear(range);
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
				this->~RequeueTimedOutTasksActorState();
				static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(false); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(false);
			this->~RequeueTimedOutTasksActorState();
			static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(lastKey != Key());
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(KeyRangeRef(range.begin, lastKey));
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
		this->~RequeueTimedOutTasksActorState();
		static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult && values,int loopDepth) 
	{
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Task task;
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Key lastKey;
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& iter : values ) {
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple t = taskBucket->timeouts.unpack(iter.key);
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key uid = t.getString(1);
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = t.getString(2);
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (uid != task.key)
															#line 9270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				for( auto& p : task.params ) {
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					tr->set(space.pack(p.key), p.value);
															#line 9278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.params.clear();
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.key = uid;
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastKey = iter.key;
															#line 9286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task.params[param] = iter.value;
															#line 9290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!values.more)
															#line 9294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task.params ) {
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(space.pack(p.key), p.value);
															#line 9302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (values.size() > 0)
															#line 9306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->clear(range);
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
				this->~RequeueTimedOutTasksActorState();
				static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(false); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(false);
			this->~RequeueTimedOutTasksActorState();
			static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(lastKey != Key());
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(KeyRangeRef(range.begin, lastKey));
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
		this->~RequeueTimedOutTasksActorState();
		static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& values,int loopDepth) 
	{
		loopDepth = a_body1cont2(values, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && values,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(values), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state > 0) static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 0;
		static_cast<RequeueTimedOutTasksActor*>(this)->ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	KeyRange range;
															#line 9409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via requeueTimedOutTasks()
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RequeueTimedOutTasksActor final : public Actor<bool>, public ActorCallback< RequeueTimedOutTasksActor, 0, Version >, public ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >, public FastAllocated<RequeueTimedOutTasksActor>, public RequeueTimedOutTasksActorState<RequeueTimedOutTasksActor> {
															#line 9414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<RequeueTimedOutTasksActor>::operator new;
	using FastAllocated<RequeueTimedOutTasksActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RequeueTimedOutTasksActor, 0, Version >;
friend struct ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >;
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RequeueTimedOutTasksActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 9426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   RequeueTimedOutTasksActorState<RequeueTimedOutTasksActor>(tr, taskBucket)
	{
		fdb_probe_actor_enter("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("requeueTimedOutTasks");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("requeueTimedOutTasks", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RequeueTimedOutTasksActor, 0, Version >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> requeueTimedOutTasks( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new RequeueTimedOutTasksActor(tr, taskBucket));
															#line 9454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 9459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via debugPrintRange()
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DebugPrintRangeActor>
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DebugPrintRangeActorState {
															#line 9465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DebugPrintRangeActorState(Reference<ReadYourWritesTransaction> const& tr,Subspace const& subspace,Key const& msg) 
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   subspace(subspace),
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   msg(msg)
															#line 9476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("debugPrintRange", reinterpret_cast<unsigned long>(this));

	}
	~DebugPrintRangeActorState() 
	{
		fdb_probe_actor_destroy("debugPrintRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(subspace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DebugPrintRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DebugPrintRangeActor*>(this)->actor_wait_state = 1;
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DebugPrintRangeActor, 0, RangeResult >*>(static_cast<DebugPrintRangeActor*>(this)));
															#line 9502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~DebugPrintRangeActorState();
		static_cast<DebugPrintRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& values,int loopDepth) 
	{
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TraceEvent("TaskBucketDebugPrintRange") .detail("Key", subspace.key()) .detail("Count", values.size()) .detail("Msg", msg);
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DebugPrintRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DebugPrintRangeActorState(); static_cast<DebugPrintRangeActor*>(this)->destroy(); return 0; }
															#line 9527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DebugPrintRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DebugPrintRangeActorState();
		static_cast<DebugPrintRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TraceEvent("TaskBucketDebugPrintRange") .detail("Key", subspace.key()) .detail("Count", values.size()) .detail("Msg", msg);
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DebugPrintRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DebugPrintRangeActorState(); static_cast<DebugPrintRangeActor*>(this)->destroy(); return 0; }
															#line 9541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DebugPrintRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DebugPrintRangeActorState();
		static_cast<DebugPrintRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& values,int loopDepth) 
	{
		loopDepth = a_body1cont1(values, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && values,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(values), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DebugPrintRangeActor*>(this)->actor_wait_state > 0) static_cast<DebugPrintRangeActor*>(this)->actor_wait_state = 0;
		static_cast<DebugPrintRangeActor*>(this)->ActorCallback< DebugPrintRangeActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< DebugPrintRangeActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("debugPrintRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debugPrintRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DebugPrintRangeActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("debugPrintRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debugPrintRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DebugPrintRangeActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("debugPrintRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debugPrintRange", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace subspace;
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Key msg;
															#line 9618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via debugPrintRange()
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DebugPrintRangeActor final : public Actor<Void>, public ActorCallback< DebugPrintRangeActor, 0, RangeResult >, public FastAllocated<DebugPrintRangeActor>, public DebugPrintRangeActorState<DebugPrintRangeActor> {
															#line 9623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DebugPrintRangeActor>::operator new;
	using FastAllocated<DebugPrintRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DebugPrintRangeActor, 0, RangeResult >;
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DebugPrintRangeActor(Reference<ReadYourWritesTransaction> const& tr,Subspace const& subspace,Key const& msg) 
															#line 9634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   DebugPrintRangeActorState<DebugPrintRangeActor>(tr, subspace, msg)
	{
		fdb_probe_actor_enter("debugPrintRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("debugPrintRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("debugPrintRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DebugPrintRangeActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> debugPrintRange( Reference<ReadYourWritesTransaction> const& tr, Subspace const& subspace, Key const& msg ) {
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new DebugPrintRangeActor(tr, subspace, msg));
															#line 9661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 9666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via extendTimeout()
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class ExtendTimeoutActor>
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutActorState {
															#line 9672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task,UpdateParams const& updateParams,Version const& newTimeoutVersion) 
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   updateParams(updateParams),
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   newTimeoutVersion(newTimeoutVersion)
															#line 9687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("extendTimeout", reinterpret_cast<unsigned long>(this));

	}
	~ExtendTimeoutActorState() 
	{
		fdb_probe_actor_destroy("extendTimeout", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = taskBucket->keepRunning(tr, task);
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 1;
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 0, Void >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~ExtendTimeoutActorState();
		static_cast<ExtendTimeoutActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		oldTimeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(task->key);
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Version> __when_expr_1 = tr->getReadVersion();
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 2;
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 1, Version >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		oldTimeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(task->key);
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Version> __when_expr_1 = tr->getReadVersion();
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 2;
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 1, Version >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutActor*>(this)->ActorCallback< ExtendTimeoutActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Version const& version,int loopDepth) 
	{
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion == invalidVersion)
															#line 9833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = version + taskBucket->timeout;
															#line 9837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		else
		{
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (newTimeoutVersion <= version)
															#line 9843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				newTimeoutVersion = version + 1;
															#line 9847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion <= task->timeoutVersion)
															#line 9852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = task->timeoutVersion + 1;
															#line 9856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		newTimeoutSpace = taskBucket->timeouts.get(newTimeoutVersion).get(task->key);
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addReadConflictRange(oldTimeoutSpace.range());
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addWriteConflictRange(newTimeoutSpace.range());
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (updateParams)
															#line 9866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task->params ) {
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(newTimeoutSpace.pack(p.key), p.value);
															#line 9874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont3(loopDepth);
		}
		else
		{
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = tr->getRange(oldTimeoutSpace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 3;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 2, RangeResult >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2(Version && version,int loopDepth) 
	{
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion == invalidVersion)
															#line 9901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = version + taskBucket->timeout;
															#line 9905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		else
		{
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (newTimeoutVersion <= version)
															#line 9911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				newTimeoutVersion = version + 1;
															#line 9915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion <= task->timeoutVersion)
															#line 9920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = task->timeoutVersion + 1;
															#line 9924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		newTimeoutSpace = taskBucket->timeouts.get(newTimeoutVersion).get(task->key);
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addReadConflictRange(oldTimeoutSpace.range());
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addWriteConflictRange(newTimeoutSpace.range());
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (updateParams)
															#line 9934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task->params ) {
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(newTimeoutSpace.pack(p.key), p.value);
															#line 9942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont3(loopDepth);
		}
		else
		{
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = tr->getRange(oldTimeoutSpace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 3;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 2, RangeResult >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1when1(Version const& version,int loopDepth) 
	{
		loopDepth = a_body1cont2(version, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Version && version,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(version), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutActor*>(this)->ActorCallback< ExtendTimeoutActor, 1, Version >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutActor, 1, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutActor, 1, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutActor, 1, Version >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(oldTimeoutSpace.range());
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<ExtendTimeoutActor*>(this)->SAV<Version>::futures) { (void)(newTimeoutVersion); this->~ExtendTimeoutActorState(); static_cast<ExtendTimeoutActor*>(this)->destroy(); return 0; }
															#line 10034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<ExtendTimeoutActor*>(this)->SAV< Version >::value()) Version(std::move(newTimeoutVersion)); // state_var_RVO
		this->~ExtendTimeoutActorState();
		static_cast<ExtendTimeoutActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(RangeResult const& params,int loopDepth) 
	{
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& kv : params ) {
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple paramKey = oldTimeoutSpace.unpack(kv.key);
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(newTimeoutSpace.pack(paramKey), kv.value);
															#line 10050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(RangeResult && params,int loopDepth) 
	{
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& kv : params ) {
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple paramKey = oldTimeoutSpace.unpack(kv.key);
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(newTimeoutSpace.pack(paramKey), kv.value);
															#line 10064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(RangeResult const& params,int loopDepth) 
	{
		loopDepth = a_body1cont10(params, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(RangeResult && params,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(params), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state > 0) static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 0;
		static_cast<ExtendTimeoutActor*>(this)->ActorCallback< ExtendTimeoutActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExtendTimeoutActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExtendTimeoutActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	UpdateParams updateParams;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Version newTimeoutVersion;
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace oldTimeoutSpace;
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace newTimeoutSpace;
															#line 10147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via extendTimeout()
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutActor final : public Actor<Version>, public ActorCallback< ExtendTimeoutActor, 0, Void >, public ActorCallback< ExtendTimeoutActor, 1, Version >, public ActorCallback< ExtendTimeoutActor, 2, RangeResult >, public FastAllocated<ExtendTimeoutActor>, public ExtendTimeoutActorState<ExtendTimeoutActor> {
															#line 10152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<ExtendTimeoutActor>::operator new;
	using FastAllocated<ExtendTimeoutActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Version>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExtendTimeoutActor, 0, Void >;
friend struct ActorCallback< ExtendTimeoutActor, 1, Version >;
friend struct ActorCallback< ExtendTimeoutActor, 2, RangeResult >;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task,UpdateParams const& updateParams,Version const& newTimeoutVersion) 
															#line 10165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Version>(),
		   ExtendTimeoutActorState<ExtendTimeoutActor>(tr, taskBucket, task, updateParams, newTimeoutVersion)
	{
		fdb_probe_actor_enter("extendTimeout", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("extendTimeout");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("extendTimeout", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExtendTimeoutActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExtendTimeoutActor, 1, Version >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExtendTimeoutActor, 2, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Version> extendTimeout( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<Task> const& task, UpdateParams const& updateParams, Version const& newTimeoutVersion ) {
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Version>(new ExtendTimeoutActor(tr, taskBucket, task, updateParams, newTimeoutVersion));
															#line 10194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};

TaskBucket::TaskBucket(const Subspace& subspace,
                       AccessSystemKeys sysAccess,
                       PriorityBatch priorityBatch,
                       LockAware lockAware)
  : cc("TaskBucket"), dispatchSlotChecksStarted("DispatchSlotChecksStarted", cc), dispatchErrors("DispatchErrors", cc),
    dispatchDoTasks("DispatchDoTasks", cc), dispatchEmptyTasks("DispatchEmptyTasks", cc),
    dispatchSlotChecksComplete("DispatchSlotChecksComplete", cc), dbgid(deterministicRandom()->randomUniqueID()),
    prefix(subspace), active(prefix.get(LiteralStringRef("ac"))), pauseKey(prefix.pack(LiteralStringRef("pause"))),
    available(prefix.get(LiteralStringRef("av"))), available_prioritized(prefix.get(LiteralStringRef("avp"))),
    timeouts(prefix.get(LiteralStringRef("to"))), timeout(CLIENT_KNOBS->TASKBUCKET_TIMEOUT_VERSIONS),
    system_access(sysAccess), priority_batch(priorityBatch), lockAware(lockAware) {}

TaskBucket::~TaskBucket() {}

Future<Void> TaskBucket::clear(Reference<ReadYourWritesTransaction> tr) {
	setOptions(tr);

	tr->clear(prefix.range());

	return Void();
}

Future<Void> TaskBucket::changePause(Reference<ReadYourWritesTransaction> tr, bool pause) {
	setOptions(tr);

	if (pause) {
		tr->set(pauseKey, StringRef());
	} else {
		tr->clear(pauseKey);
	}

	return Void();
}

Key TaskBucket::addTask(Reference<ReadYourWritesTransaction> tr, Reference<Task> task) {
	setOptions(tr);

	Key key(deterministicRandom()->randomUniqueID().toString());

	Subspace taskSpace;

	// If scheduledVersion is valid then place the task directly into the timeout
	// space for its scheduled time, otherwise place it in the available space by priority.
	Version scheduledVersion = ReservedTaskParams::scheduledVersion().getOrDefault(task, invalidVersion);
	if (scheduledVersion != invalidVersion) {
		taskSpace = timeouts.get(scheduledVersion).get(key);
	} else {
		taskSpace = getAvailableSpace(task->getPriority()).get(key);
	}

	for (auto& param : task->params)
		tr->set(taskSpace.pack(param.key), param.value);

	tr->atomicOp(prefix.pack(LiteralStringRef("task_count")),
	             LiteralStringRef("\x01\x00\x00\x00\x00\x00\x00\x00"),
	             MutationRef::AddValue);

	return key;
}

void TaskBucket::setValidationCondition(Reference<Task> task, KeyRef vKey, KeyRef vValue) {
	task->params[Task::reservedTaskParamValidKey] = vKey;
	task->params[Task::reservedTaskParamValidValue] = vValue;
}

															#line 10265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
namespace {
// This generated class is to be used only via actorAddTask()
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class ActorAddTaskActor>
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ActorAddTaskActorState {
															#line 10272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ActorAddTaskActorState(TaskBucket* const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tb(tb),
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(tr),
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   validationKey(validationKey)
															#line 10285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("actorAddTask", reinterpret_cast<unsigned long>(this));

	}
	~ActorAddTaskActorState() 
	{
		fdb_probe_actor_destroy("actorAddTask", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tb->setOptions(tr);
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(validationKey);
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ActorAddTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ActorAddTaskActor*>(this)->actor_wait_state = 1;
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorAddTaskActor, 0, Optional<Value> >*>(static_cast<ActorAddTaskActor*>(this)));
															#line 10309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~ActorAddTaskActorState();
		static_cast<ActorAddTaskActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& validationValue,int loopDepth) 
	{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 10332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 10338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TaskBucket::setValidationCondition(task, validationKey, validationValue.get());
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<ActorAddTaskActor*>(this)->SAV<Key>::futures) { (void)(tb->addTask(tr, task)); this->~ActorAddTaskActorState(); static_cast<ActorAddTaskActor*>(this)->destroy(); return 0; }
															#line 10344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<ActorAddTaskActor*>(this)->SAV< Key >::value()) Key(tb->addTask(tr, task));
		this->~ActorAddTaskActorState();
		static_cast<ActorAddTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && validationValue,int loopDepth) 
	{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 10356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 10362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TaskBucket::setValidationCondition(task, validationKey, validationValue.get());
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<ActorAddTaskActor*>(this)->SAV<Key>::futures) { (void)(tb->addTask(tr, task)); this->~ActorAddTaskActorState(); static_cast<ActorAddTaskActor*>(this)->destroy(); return 0; }
															#line 10368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<ActorAddTaskActor*>(this)->SAV< Key >::value()) Key(tb->addTask(tr, task));
		this->~ActorAddTaskActorState();
		static_cast<ActorAddTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& validationValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(validationValue, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && validationValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(validationValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorAddTaskActor*>(this)->actor_wait_state > 0) static_cast<ActorAddTaskActor*>(this)->actor_wait_state = 0;
		static_cast<ActorAddTaskActor*>(this)->ActorCallback< ActorAddTaskActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ActorAddTaskActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("actorAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ActorAddTaskActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("actorAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ActorAddTaskActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("actorAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskBucket* tb;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	KeyRef validationKey;
															#line 10447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via actorAddTask()
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ActorAddTaskActor final : public Actor<Key>, public ActorCallback< ActorAddTaskActor, 0, Optional<Value> >, public FastAllocated<ActorAddTaskActor>, public ActorAddTaskActorState<ActorAddTaskActor> {
															#line 10452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<ActorAddTaskActor>::operator new;
	using FastAllocated<ActorAddTaskActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Key>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorAddTaskActor, 0, Optional<Value> >;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ActorAddTaskActor(TaskBucket* const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 10463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Key>(),
		   ActorAddTaskActorState<ActorAddTaskActor>(tb, tr, task, validationKey)
	{
		fdb_probe_actor_enter("actorAddTask", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorAddTask");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorAddTask", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ActorAddTaskActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Key> actorAddTask( TaskBucket* const& tb, Reference<ReadYourWritesTransaction> const& tr, Reference<Task> const& task, KeyRef const& validationKey ) {
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Key>(new ActorAddTaskActor(tb, tr, task, validationKey));
															#line 10491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

Future<Key> TaskBucket::addTask(Reference<ReadYourWritesTransaction> tr, Reference<Task> task, KeyRef validationKey) {
	return actorAddTask(this, tr, task, validationKey);
}

Key TaskBucket::addTask(Reference<ReadYourWritesTransaction> tr,
                        Reference<Task> task,
                        KeyRef validationKey,
                        KeyRef validationValue) {
	setValidationCondition(task, validationKey, validationValue);
	return addTask(tr, task);
}

Future<Reference<Task>> TaskBucket::getOne(Reference<ReadYourWritesTransaction> tr) {
	return TaskBucketImpl::getOne(tr, Reference<TaskBucket>::addRef(this));
}

Future<bool> TaskBucket::doOne(Database cx, Reference<FutureBucket> futureBucket) {
	return TaskBucketImpl::doOne(cx, Reference<TaskBucket>::addRef(this), futureBucket);
}

Future<bool> TaskBucket::doTask(Database cx, Reference<FutureBucket> futureBucket, Reference<Task> task) {
	return TaskBucketImpl::doTask(cx, Reference<TaskBucket>::addRef(this), futureBucket, task);
}

Future<Void> TaskBucket::run(Database cx,
                             Reference<FutureBucket> futureBucket,
                             std::shared_ptr<double const> pollDelay,
                             int maxConcurrentTasks) {
	return TaskBucketImpl::run(cx, Reference<TaskBucket>::addRef(this), futureBucket, pollDelay, maxConcurrentTasks);
}

Future<Void> TaskBucket::watchPaused(Database cx, Reference<AsyncVar<bool>> paused) {
	return TaskBucketImpl::watchPaused(cx, Reference<TaskBucket>::addRef(this), paused);
}

Future<bool> TaskBucket::isEmpty(Reference<ReadYourWritesTransaction> tr) {
	return TaskBucketImpl::isEmpty(tr, Reference<TaskBucket>::addRef(this));
}

Future<Void> TaskBucket::finish(Reference<ReadYourWritesTransaction> tr, Reference<Task> task) {
	setOptions(tr);

	Tuple t;
	t.append(task->timeoutVersion);
	t.append(task->key);

	tr->atomicOp(prefix.pack(LiteralStringRef("task_count")),
	             LiteralStringRef("\xff\xff\xff\xff\xff\xff\xff\xff"),
	             MutationRef::AddValue);
	tr->clear(timeouts.range(t));

	return Void();
}

Future<Version> TaskBucket::extendTimeout(Reference<ReadYourWritesTransaction> tr,
                                          Reference<Task> task,
                                          UpdateParams updateParams,
                                          Version newTimeoutVersion) {
	return TaskBucketImpl::extendTimeout(
	    tr, Reference<TaskBucket>::addRef(this), task, updateParams, newTimeoutVersion);
}

Future<bool> TaskBucket::isFinished(Reference<ReadYourWritesTransaction> tr, Reference<Task> task) {
	return TaskBucketImpl::isFinished(tr, Reference<TaskBucket>::addRef(this), task);
}

Future<bool> TaskBucket::isVerified(Reference<ReadYourWritesTransaction> tr, Reference<Task> task) {
	return TaskBucketImpl::taskVerify(Reference<TaskBucket>::addRef(this), tr, task);
}

Future<bool> TaskBucket::checkActive(Database cx) {
	return TaskBucketImpl::checkActive(cx, Reference<TaskBucket>::addRef(this));
}

Future<int64_t> TaskBucket::getTaskCount(Reference<ReadYourWritesTransaction> tr) {
	return TaskBucketImpl::getTaskCount(tr, Reference<TaskBucket>::addRef(this));
}

Future<Void> TaskBucket::watchTaskCount(Reference<ReadYourWritesTransaction> tr) {
	return tr->watch(prefix.pack(LiteralStringRef("task_count")));
}

Future<Void> TaskBucket::debugPrintRange(Reference<ReadYourWritesTransaction> tr, Subspace subspace, Key msg) {
	return TaskBucketImpl::debugPrintRange(tr, subspace, msg);
}

class FutureBucketImpl {
public:
																#line 10584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isEmpty()
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsEmptyActor1>
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActor1State {
															#line 10590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActor1State(Reference<ReadYourWritesTransaction> const& tr,Reference<FutureBucket> const& futureBucket) 
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket)
															#line 10599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("isEmpty", reinterpret_cast<unsigned long>(this));

	}
	~IsEmptyActor1State() 
	{
		fdb_probe_actor_destroy("isEmpty", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			futureBucket->setOptions(tr);
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Key> __when_expr_0 = tr->getKey(lastLessOrEqual(futureBucket->prefix.pack(maxUIDKey)));
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<IsEmptyActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsEmptyActor1*>(this)->actor_wait_state = 1;
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsEmptyActor1, 0, Key >*>(static_cast<IsEmptyActor1*>(this)));
															#line 10623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~IsEmptyActor1State();
		static_cast<IsEmptyActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Key const& lastKey,int loopDepth) 
	{
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor1*>(this)->SAV<bool>::futures) { (void)(!futureBucket->prefix.contains(lastKey)); this->~IsEmptyActor1State(); static_cast<IsEmptyActor1*>(this)->destroy(); return 0; }
															#line 10646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsEmptyActor1*>(this)->SAV< bool >::value()) bool(!futureBucket->prefix.contains(lastKey));
		this->~IsEmptyActor1State();
		static_cast<IsEmptyActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Key && lastKey,int loopDepth) 
	{
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor1*>(this)->SAV<bool>::futures) { (void)(!futureBucket->prefix.contains(lastKey)); this->~IsEmptyActor1State(); static_cast<IsEmptyActor1*>(this)->destroy(); return 0; }
															#line 10658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsEmptyActor1*>(this)->SAV< bool >::value()) bool(!futureBucket->prefix.contains(lastKey));
		this->~IsEmptyActor1State();
		static_cast<IsEmptyActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Key const& lastKey,int loopDepth) 
	{
		loopDepth = a_body1cont1(lastKey, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && lastKey,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(lastKey), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IsEmptyActor1*>(this)->actor_wait_state > 0) static_cast<IsEmptyActor1*>(this)->actor_wait_state = 0;
		static_cast<IsEmptyActor1*>(this)->ActorCallback< IsEmptyActor1, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< IsEmptyActor1, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IsEmptyActor1, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IsEmptyActor1, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 10733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isEmpty()
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActor1 final : public Actor<bool>, public ActorCallback< IsEmptyActor1, 0, Key >, public FastAllocated<IsEmptyActor1>, public IsEmptyActor1State<IsEmptyActor1> {
															#line 10738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsEmptyActor1>::operator new;
	using FastAllocated<IsEmptyActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsEmptyActor1, 0, Key >;
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActor1(Reference<ReadYourWritesTransaction> const& tr,Reference<FutureBucket> const& futureBucket) 
															#line 10749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   IsEmptyActor1State<IsEmptyActor1>(tr, futureBucket)
	{
		fdb_probe_actor_enter("isEmpty", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("isEmpty");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("isEmpty", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IsEmptyActor1, 0, Key >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isEmpty( Reference<ReadYourWritesTransaction> const& tr, Reference<FutureBucket> const& futureBucket ) {
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsEmptyActor1(tr, futureBucket));
															#line 10776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};

FutureBucket::FutureBucket(const Subspace& subspace, AccessSystemKeys sysAccess, LockAware lockAware)
  : prefix(subspace), system_access(sysAccess), lockAware(lockAware) {}

FutureBucket::~FutureBucket() {}

Future<Void> FutureBucket::clear(Reference<ReadYourWritesTransaction> tr) {
	setOptions(tr);
	tr->clear(prefix.range());

	return Void();
}

Reference<TaskFuture> FutureBucket::future(Reference<ReadYourWritesTransaction> tr) {
	setOptions(tr);

	auto taskFuture = makeReference<TaskFuture>(Reference<FutureBucket>::addRef(this));
	taskFuture->addBlock(tr, StringRef());

	return taskFuture;
}

Future<bool> FutureBucket::isEmpty(Reference<ReadYourWritesTransaction> tr) {
	return FutureBucketImpl::isEmpty(tr, Reference<FutureBucket>::addRef(this));
}

Reference<TaskFuture> FutureBucket::unpack(Key key) {
	return makeReference<TaskFuture>(Reference<FutureBucket>::addRef(this), key);
}

class TaskFutureImpl {
public:
																#line 10813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via join()
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class JoinActor>
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinActorState {
															#line 10819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   vectorFuture(vectorFuture)
															#line 10832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("join", reinterpret_cast<unsigned long>(this));

	}
	~JoinActorState() 
	{
		fdb_probe_actor_destroy("join", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = isSet(tr, taskFuture);
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<JoinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<JoinActor*>(this)->actor_wait_state = 1;
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< JoinActor, 0, bool >*>(static_cast<JoinActor*>(this)));
															#line 10856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~JoinActorState();
		static_cast<JoinActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& is_set,int loopDepth) 
	{
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 10879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 10883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~JoinActorState();
			static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->blocks.pack(StringRef()));
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = _join(tr, taskBucket, taskFuture, vectorFuture);
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<JoinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<JoinActor*>(this)->actor_wait_state = 2;
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< JoinActor, 1, Void >*>(static_cast<JoinActor*>(this)));
															#line 10900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool && is_set,int loopDepth) 
	{
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 10909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 10913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~JoinActorState();
			static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->blocks.pack(StringRef()));
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = _join(tr, taskBucket, taskFuture, vectorFuture);
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<JoinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<JoinActor*>(this)->actor_wait_state = 2;
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< JoinActor, 1, Void >*>(static_cast<JoinActor*>(this)));
															#line 10930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(bool const& is_set,int loopDepth) 
	{
		loopDepth = a_body1cont1(is_set, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && is_set,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(is_set), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<JoinActor*>(this)->actor_wait_state > 0) static_cast<JoinActor*>(this)->actor_wait_state = 0;
		static_cast<JoinActor*>(this)->ActorCallback< JoinActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< JoinActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< JoinActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< JoinActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 11002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~JoinActorState();
		static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 11014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~JoinActorState();
		static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<JoinActor*>(this)->actor_wait_state > 0) static_cast<JoinActor*>(this)->actor_wait_state = 0;
		static_cast<JoinActor*>(this)->ActorCallback< JoinActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< JoinActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< JoinActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< JoinActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Reference<TaskFuture>> vectorFuture;
															#line 11093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via join()
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinActor final : public Actor<Void>, public ActorCallback< JoinActor, 0, bool >, public ActorCallback< JoinActor, 1, Void >, public FastAllocated<JoinActor>, public JoinActorState<JoinActor> {
															#line 11098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<JoinActor>::operator new;
	using FastAllocated<JoinActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< JoinActor, 0, bool >;
friend struct ActorCallback< JoinActor, 1, Void >;
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 11110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   JoinActorState<JoinActor>(tr, taskBucket, taskFuture, vectorFuture)
	{
		fdb_probe_actor_enter("join", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("join");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("join", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< JoinActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< JoinActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> join( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, std::vector<Reference<TaskFuture>> const& vectorFuture ) {
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new JoinActor(tr, taskBucket, taskFuture, vectorFuture));
															#line 11138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via _join()
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class _joinActor>
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _joinActorState {
															#line 11149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_joinActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   vectorFuture(vectorFuture)
															#line 11162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("_join", reinterpret_cast<unsigned long>(this));

	}
	~_joinActorState() 
	{
		fdb_probe_actor_destroy("_join", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			std::vector<Future<Void>> onSetFutures;
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int i = 0;i < vectorFuture.size();++i) {
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key key = StringRef(deterministicRandom()->randomUniqueID().toString());
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				taskFuture->addBlock(tr, key);
															#line 1110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				auto task = makeReference<Task>();
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[Task::reservedTaskParamKeyType] = LiteralStringRef("UnblockFuture");
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[Task::reservedTaskParamKeyFuture] = taskFuture->key;
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[Task::reservedTaskParamKeyBlockID] = key;
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				onSetFutures.push_back(vectorFuture[i]->onSet(tr, taskBucket, task));
															#line 11193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(onSetFutures);
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_joinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_joinActor*>(this)->actor_wait_state = 1;
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _joinActor, 0, Void >*>(static_cast<_joinActor*>(this)));
															#line 11204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~_joinActorState();
		static_cast<_joinActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<_joinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_joinActorState(); static_cast<_joinActor*>(this)->destroy(); return 0; }
															#line 11227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<_joinActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_joinActorState();
		static_cast<_joinActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<_joinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_joinActorState(); static_cast<_joinActor*>(this)->destroy(); return 0; }
															#line 11239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<_joinActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_joinActorState();
		static_cast<_joinActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<_joinActor*>(this)->actor_wait_state > 0) static_cast<_joinActor*>(this)->actor_wait_state = 0;
		static_cast<_joinActor*>(this)->ActorCallback< _joinActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< _joinActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("_join", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_join", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _joinActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("_join", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_join", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _joinActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("_join", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_join", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Reference<TaskFuture>> vectorFuture;
															#line 11318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via _join()
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _joinActor final : public Actor<Void>, public ActorCallback< _joinActor, 0, Void >, public FastAllocated<_joinActor>, public _joinActorState<_joinActor> {
															#line 11323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<_joinActor>::operator new;
	using FastAllocated<_joinActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _joinActor, 0, Void >;
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_joinActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 11334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   _joinActorState<_joinActor>(tr, taskBucket, taskFuture, vectorFuture)
	{
		fdb_probe_actor_enter("_join", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_join");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_join", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _joinActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> _join( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, std::vector<Reference<TaskFuture>> const& vectorFuture ) {
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new _joinActor(tr, taskBucket, taskFuture, vectorFuture));
															#line 11361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isSet()
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsSetActor>
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsSetActorState {
															#line 11372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsSetActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskFuture> const& taskFuture) 
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 11381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("isSet", reinterpret_cast<unsigned long>(this));

	}
	~IsSetActorState() 
	{
		fdb_probe_actor_destroy("isSet", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(taskFuture->blocks.range(), 1);
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<IsSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsSetActor*>(this)->actor_wait_state = 1;
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsSetActor, 0, RangeResult >*>(static_cast<IsSetActor*>(this)));
															#line 11405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~IsSetActorState();
		static_cast<IsSetActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& values,int loopDepth) 
	{
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 11428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsSetActorState();
			static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsSetActorState();
		static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 11452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsSetActorState();
			static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsSetActorState();
		static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& values,int loopDepth) 
	{
		loopDepth = a_body1cont1(values, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && values,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(values), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IsSetActor*>(this)->actor_wait_state > 0) static_cast<IsSetActor*>(this)->actor_wait_state = 0;
		static_cast<IsSetActor*>(this)->ActorCallback< IsSetActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< IsSetActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("isSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isSet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IsSetActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("isSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isSet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IsSetActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("isSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isSet", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 11539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isSet()
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsSetActor final : public Actor<bool>, public ActorCallback< IsSetActor, 0, RangeResult >, public FastAllocated<IsSetActor>, public IsSetActorState<IsSetActor> {
															#line 11544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsSetActor>::operator new;
	using FastAllocated<IsSetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsSetActor, 0, RangeResult >;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsSetActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskFuture> const& taskFuture) 
															#line 11555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<bool>(),
		   IsSetActorState<IsSetActor>(tr, taskFuture)
	{
		fdb_probe_actor_enter("isSet", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("isSet");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("isSet", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IsSetActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isSet( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskFuture> const& taskFuture ) {
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsSetActor(tr, taskFuture));
															#line 11582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via onSet()
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class OnSetActor>
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetActorState {
															#line 11593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 11606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("onSet", reinterpret_cast<unsigned long>(this));

	}
	~OnSetActorState() 
	{
		fdb_probe_actor_destroy("onSet", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = isSet(tr, taskFuture);
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSetActor*>(this)->actor_wait_state = 1;
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSetActor, 0, bool >*>(static_cast<OnSetActor*>(this)));
															#line 11630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~OnSetActorState();
		static_cast<OnSetActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& is_set,int loopDepth) 
	{
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 11653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = performAction(tr, taskBucket, taskFuture, task);
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnSetActor*>(this)->actor_wait_state = 2;
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetActor, 1, Void >*>(static_cast<OnSetActor*>(this)));
															#line 11666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace callbackSpace = taskFuture->callbacks.get(StringRef(deterministicRandom()->randomUniqueID().toString()));
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& v : task->params ) {
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(callbackSpace.pack(v.key), v.value);
															#line 11679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(bool && is_set,int loopDepth) 
	{
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 11690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = performAction(tr, taskBucket, taskFuture, task);
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnSetActor*>(this)->actor_wait_state = 2;
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetActor, 1, Void >*>(static_cast<OnSetActor*>(this)));
															#line 11703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TEST(true);
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace callbackSpace = taskFuture->callbacks.get(StringRef(deterministicRandom()->randomUniqueID().toString()));
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& v : task->params ) {
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(callbackSpace.pack(v.key), v.value);
															#line 11716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(bool const& is_set,int loopDepth) 
	{
		loopDepth = a_body1cont1(is_set, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && is_set,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(is_set), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OnSetActor*>(this)->actor_wait_state > 0) static_cast<OnSetActor*>(this)->actor_wait_state = 0;
		static_cast<OnSetActor*>(this)->ActorCallback< OnSetActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< OnSetActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSetActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSetActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetActorState(); static_cast<OnSetActor*>(this)->destroy(); return 0; }
															#line 11790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetActorState();
		static_cast<OnSetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

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
		if (static_cast<OnSetActor*>(this)->actor_wait_state > 0) static_cast<OnSetActor*>(this)->actor_wait_state = 0;
		static_cast<OnSetActor*>(this)->ActorCallback< OnSetActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSetActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnSetActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnSetActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 11881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via onSet()
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetActor final : public Actor<Void>, public ActorCallback< OnSetActor, 0, bool >, public ActorCallback< OnSetActor, 1, Void >, public FastAllocated<OnSetActor>, public OnSetActorState<OnSetActor> {
															#line 11886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<OnSetActor>::operator new;
	using FastAllocated<OnSetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSetActor, 0, bool >;
friend struct ActorCallback< OnSetActor, 1, Void >;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 11898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   OnSetActorState<OnSetActor>(tr, taskBucket, taskFuture, task)
	{
		fdb_probe_actor_enter("onSet", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSet");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSet", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSetActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnSetActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> onSet( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task ) {
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new OnSetActor(tr, taskBucket, taskFuture, task));
															#line 11926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via set()
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class SetActor>
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class SetActorState {
															#line 11937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	SetActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 11948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("set", reinterpret_cast<unsigned long>(this));

	}
	~SetActorState() 
	{
		fdb_probe_actor_destroy("set", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->clear(taskFuture->blocks.range());
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = performAllActions(tr, taskBucket, taskFuture);
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<SetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetActor*>(this)->actor_wait_state = 1;
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetActor, 0, Void >*>(static_cast<SetActor*>(this)));
															#line 11974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~SetActorState();
		static_cast<SetActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<SetActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetActorState(); static_cast<SetActor*>(this)->destroy(); return 0; }
															#line 11997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<SetActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetActorState();
		static_cast<SetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<SetActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetActorState(); static_cast<SetActor*>(this)->destroy(); return 0; }
															#line 12009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<SetActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetActorState();
		static_cast<SetActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SetActor*>(this)->actor_wait_state > 0) static_cast<SetActor*>(this)->actor_wait_state = 0;
		static_cast<SetActor*>(this)->ActorCallback< SetActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("set", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("set", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("set", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("set", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("set", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("set", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 12086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via set()
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class SetActor final : public Actor<Void>, public ActorCallback< SetActor, 0, Void >, public FastAllocated<SetActor>, public SetActorState<SetActor> {
															#line 12091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<SetActor>::operator new;
	using FastAllocated<SetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetActor, 0, Void >;
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	SetActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 12102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   SetActorState<SetActor>(tr, taskBucket, taskFuture)
	{
		fdb_probe_actor_enter("set", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("set");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("set", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> set( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture ) {
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new SetActor(tr, taskBucket, taskFuture));
															#line 12129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via performAction()
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class PerformActionActor>
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformActionActorState {
															#line 12140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformActionActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 12153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("performAction", reinterpret_cast<unsigned long>(this));

	}
	~PerformActionActorState() 
	{
		fdb_probe_actor_destroy("performAction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task && TaskFuncBase::isValidTask(task))
															#line 12170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Reference<TaskFuncBase> taskFunc = TaskFuncBase::create(task->params[Task::reservedTaskParamKeyType]);
															#line 1175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskFunc.getPtr())
															#line 12176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					StrictFuture<Void> __when_expr_0 = taskFunc->finish(tr, taskBucket, taskFuture->futureBucket, task);
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					if (static_cast<PerformActionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<PerformActionActor*>(this)->actor_wait_state = 1;
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PerformActionActor, 0, Void >*>(static_cast<PerformActionActor*>(this)));
															#line 12187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1cont2(loopDepth);
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
		this->~PerformActionActorState();
		static_cast<PerformActionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<PerformActionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PerformActionActorState(); static_cast<PerformActionActor*>(this)->destroy(); return 0; }
															#line 12220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<PerformActionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PerformActionActorState();
		static_cast<PerformActionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PerformActionActor*>(this)->actor_wait_state > 0) static_cast<PerformActionActor*>(this)->actor_wait_state = 0;
		static_cast<PerformActionActor*>(this)->ActorCallback< PerformActionActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PerformActionActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("performAction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PerformActionActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("performAction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PerformActionActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("performAction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAction", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 12317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via performAction()
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformActionActor final : public Actor<Void>, public ActorCallback< PerformActionActor, 0, Void >, public FastAllocated<PerformActionActor>, public PerformActionActorState<PerformActionActor> {
															#line 12322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<PerformActionActor>::operator new;
	using FastAllocated<PerformActionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PerformActionActor, 0, Void >;
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformActionActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 12333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   PerformActionActorState<PerformActionActor>(tr, taskBucket, taskFuture, task)
	{
		fdb_probe_actor_enter("performAction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("performAction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("performAction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PerformActionActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> performAction( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task ) {
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new PerformActionActor(tr, taskBucket, taskFuture, task));
															#line 12360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via performAllActions()
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class PerformAllActionsActor>
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformAllActionsActorState {
															#line 12371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformAllActionsActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 12382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("performAllActions", reinterpret_cast<unsigned long>(this));

	}
	~PerformAllActionsActorState() 
	{
		fdb_probe_actor_destroy("performAllActions", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(taskFuture->callbacks.range(), CLIENT_KNOBS->TOO_MANY);
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 1;
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PerformAllActionsActor, 0, RangeResult >*>(static_cast<PerformAllActionsActor*>(this)));
															#line 12406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~PerformAllActionsActorState();
		static_cast<PerformAllActionsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& values,int loopDepth) 
	{
															#line 1189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->callbacks.range());
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		std::vector<Future<Void>> actions;
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() != 0)
															#line 12433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task = Reference<Task>(new Task());
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key lastTaskID;
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& s : values ) {
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Tuple t = taskFuture->callbacks.unpack(s.key);
															#line 1198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key taskID = t.getString(0);
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key key = t.getString(1);
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskID.size() != 0 && taskID != lastTaskID)
															#line 12449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					task = makeReference<Task>();
															#line 12455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[key] = s.value;
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastTaskID = taskID;
															#line 12461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 1210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 12465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(actions);
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 2;
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PerformAllActionsActor, 1, Void >*>(static_cast<PerformAllActionsActor*>(this)));
															#line 12476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 1189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->callbacks.range());
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		std::vector<Future<Void>> actions;
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() != 0)
															#line 12489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task = Reference<Task>(new Task());
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key lastTaskID;
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& s : values ) {
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Tuple t = taskFuture->callbacks.unpack(s.key);
															#line 1198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key taskID = t.getString(0);
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key key = t.getString(1);
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskID.size() != 0 && taskID != lastTaskID)
															#line 12505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					task = makeReference<Task>();
															#line 12511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[key] = s.value;
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastTaskID = taskID;
															#line 12517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 1210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 12521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(actions);
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 2;
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PerformAllActionsActor, 1, Void >*>(static_cast<PerformAllActionsActor*>(this)));
															#line 12532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& values,int loopDepth) 
	{
		loopDepth = a_body1cont1(values, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && values,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(values), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state > 0) static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 0;
		static_cast<PerformAllActionsActor*>(this)->ActorCallback< PerformAllActionsActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< PerformAllActionsActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PerformAllActionsActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PerformAllActionsActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<PerformAllActionsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PerformAllActionsActorState(); static_cast<PerformAllActionsActor*>(this)->destroy(); return 0; }
															#line 12604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<PerformAllActionsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PerformAllActionsActorState();
		static_cast<PerformAllActionsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<PerformAllActionsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PerformAllActionsActorState(); static_cast<PerformAllActionsActor*>(this)->destroy(); return 0; }
															#line 12616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<PerformAllActionsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PerformAllActionsActorState();
		static_cast<PerformAllActionsActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state > 0) static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 0;
		static_cast<PerformAllActionsActor*>(this)->ActorCallback< PerformAllActionsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PerformAllActionsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PerformAllActionsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PerformAllActionsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 12695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via performAllActions()
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformAllActionsActor final : public Actor<Void>, public ActorCallback< PerformAllActionsActor, 0, RangeResult >, public ActorCallback< PerformAllActionsActor, 1, Void >, public FastAllocated<PerformAllActionsActor>, public PerformAllActionsActorState<PerformAllActionsActor> {
															#line 12700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<PerformAllActionsActor>::operator new;
	using FastAllocated<PerformAllActionsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PerformAllActionsActor, 0, RangeResult >;
friend struct ActorCallback< PerformAllActionsActor, 1, Void >;
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformAllActionsActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 12712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   PerformAllActionsActorState<PerformAllActionsActor>(tr, taskBucket, taskFuture)
	{
		fdb_probe_actor_enter("performAllActions", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("performAllActions");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("performAllActions", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PerformAllActionsActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PerformAllActionsActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> performAllActions( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture ) {
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new PerformAllActionsActor(tr, taskBucket, taskFuture));
															#line 12740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via onSetAddTask()
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class OnSetAddTaskActor>
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActorState {
															#line 12751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 12764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("onSetAddTask", reinterpret_cast<unsigned long>(this));

	}
	~OnSetAddTaskActorState() 
	{
		fdb_probe_actor_destroy("onSetAddTask", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[Task::reservedTaskParamKeyAddTask] = task->params[Task::reservedTaskParamKeyType];
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[Task::reservedTaskParamKeyType] = LiteralStringRef("AddTask");
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = onSet(tr, taskBucket, taskFuture, task);
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetAddTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSetAddTaskActor*>(this)->actor_wait_state = 1;
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor, 0, Void >*>(static_cast<OnSetAddTaskActor*>(this)));
															#line 12792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~OnSetAddTaskActorState();
		static_cast<OnSetAddTaskActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActorState(); static_cast<OnSetAddTaskActor*>(this)->destroy(); return 0; }
															#line 12815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetAddTaskActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetAddTaskActorState();
		static_cast<OnSetAddTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActorState(); static_cast<OnSetAddTaskActor*>(this)->destroy(); return 0; }
															#line 12827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetAddTaskActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetAddTaskActorState();
		static_cast<OnSetAddTaskActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<OnSetAddTaskActor*>(this)->actor_wait_state > 0) static_cast<OnSetAddTaskActor*>(this)->actor_wait_state = 0;
		static_cast<OnSetAddTaskActor*>(this)->ActorCallback< OnSetAddTaskActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSetAddTaskActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSetAddTaskActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSetAddTaskActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 12906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via onSetAddTask()
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActor final : public Actor<Void>, public ActorCallback< OnSetAddTaskActor, 0, Void >, public FastAllocated<OnSetAddTaskActor>, public OnSetAddTaskActorState<OnSetAddTaskActor> {
															#line 12911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<OnSetAddTaskActor>::operator new;
	using FastAllocated<OnSetAddTaskActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSetAddTaskActor, 0, Void >;
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 12922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   OnSetAddTaskActorState<OnSetAddTaskActor>(tr, taskBucket, taskFuture, task)
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSetAddTask");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSetAddTaskActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> onSetAddTask( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task ) {
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new OnSetAddTaskActor(tr, taskBucket, taskFuture, task));
															#line 12949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via onSetAddTask()
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class OnSetAddTaskActor1>
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActor1State {
															#line 12960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActor1State(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   validationKey(validationKey)
															#line 12975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("onSetAddTask", reinterpret_cast<unsigned long>(this));

	}
	~OnSetAddTaskActor1State() 
	{
		fdb_probe_actor_destroy("onSetAddTask", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(validationKey);
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 1;
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >*>(static_cast<OnSetAddTaskActor1*>(this)));
															#line 12999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~OnSetAddTaskActor1State();
		static_cast<OnSetAddTaskActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& validationValue,int loopDepth) 
	{
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 13022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketOnSetAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 1244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 13028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidKey] = validationKey;
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidValue] = validationValue.get();
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = onSetAddTask(tr, taskBucket, taskFuture, task);
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 2;
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor1, 1, Void >*>(static_cast<OnSetAddTaskActor1*>(this)));
															#line 13043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && validationValue,int loopDepth) 
	{
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 13052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketOnSetAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 1244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 13058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidKey] = validationKey;
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidValue] = validationValue.get();
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = onSetAddTask(tr, taskBucket, taskFuture, task);
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 2;
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor1, 1, Void >*>(static_cast<OnSetAddTaskActor1*>(this)));
															#line 13073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& validationValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(validationValue, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && validationValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(validationValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state > 0) static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 0;
		static_cast<OnSetAddTaskActor1*>(this)->ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActor1State(); static_cast<OnSetAddTaskActor1*>(this)->destroy(); return 0; }
															#line 13145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetAddTaskActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetAddTaskActor1State();
		static_cast<OnSetAddTaskActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActor1State(); static_cast<OnSetAddTaskActor1*>(this)->destroy(); return 0; }
															#line 13157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetAddTaskActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetAddTaskActor1State();
		static_cast<OnSetAddTaskActor1*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state > 0) static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 0;
		static_cast<OnSetAddTaskActor1*>(this)->ActorCallback< OnSetAddTaskActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSetAddTaskActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnSetAddTaskActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnSetAddTaskActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	KeyRef validationKey;
															#line 13238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via onSetAddTask()
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActor1 final : public Actor<Void>, public ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >, public ActorCallback< OnSetAddTaskActor1, 1, Void >, public FastAllocated<OnSetAddTaskActor1>, public OnSetAddTaskActor1State<OnSetAddTaskActor1> {
															#line 13243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<OnSetAddTaskActor1>::operator new;
	using FastAllocated<OnSetAddTaskActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >;
friend struct ActorCallback< OnSetAddTaskActor1, 1, Void >;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActor1(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 13255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Void>(),
		   OnSetAddTaskActor1State<OnSetAddTaskActor1>(tr, taskBucket, taskFuture, task, validationKey)
	{
		fdb_probe_actor_enter("onSetAddTask", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSetAddTask");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSetAddTask", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnSetAddTaskActor1, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> onSetAddTask( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task, KeyRef const& validationKey ) {
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new OnSetAddTaskActor1(tr, taskBucket, taskFuture, task, validationKey));
															#line 13283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	static Future<Void> onSetAddTask(Reference<ReadYourWritesTransaction> tr,
	                                 Reference<TaskBucket> taskBucket,
	                                 Reference<TaskFuture> taskFuture,
	                                 Reference<Task> task,
	                                 KeyRef validationKey,
	                                 KeyRef validationValue) {
		taskFuture->futureBucket->setOptions(tr);

		task->params[Task::reservedTaskParamValidKey] = validationKey;
		task->params[Task::reservedTaskParamValidValue] = validationValue;

		return onSetAddTask(tr, taskBucket, taskFuture, task);
	}

																#line 13302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via joinedFuture()
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class JoinedFutureActor>
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinedFutureActorState {
															#line 13308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinedFutureActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 13319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("joinedFuture", reinterpret_cast<unsigned long>(this));

	}
	~JoinedFutureActorState() 
	{
		fdb_probe_actor_destroy("joinedFuture", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			std::vector<Reference<TaskFuture>> vectorFuture;
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			future = taskFuture->futureBucket->future(tr);
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			vectorFuture.push_back(future);
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = join(tr, taskBucket, taskFuture, vectorFuture);
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<JoinedFutureActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<JoinedFutureActor*>(this)->actor_wait_state = 1;
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< JoinedFutureActor, 0, Void >*>(static_cast<JoinedFutureActor*>(this)));
															#line 13349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~JoinedFutureActorState();
		static_cast<JoinedFutureActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinedFutureActor*>(this)->SAV<Reference<TaskFuture>>::futures) { (void)(future); this->~JoinedFutureActorState(); static_cast<JoinedFutureActor*>(this)->destroy(); return 0; }
															#line 13372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<JoinedFutureActor*>(this)->SAV< Reference<TaskFuture> >::value()) Reference<TaskFuture>(std::move(future)); // state_var_RVO
		this->~JoinedFutureActorState();
		static_cast<JoinedFutureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinedFutureActor*>(this)->SAV<Reference<TaskFuture>>::futures) { (void)(future); this->~JoinedFutureActorState(); static_cast<JoinedFutureActor*>(this)->destroy(); return 0; }
															#line 13384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<JoinedFutureActor*>(this)->SAV< Reference<TaskFuture> >::value()) Reference<TaskFuture>(std::move(future)); // state_var_RVO
		this->~JoinedFutureActorState();
		static_cast<JoinedFutureActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<JoinedFutureActor*>(this)->actor_wait_state > 0) static_cast<JoinedFutureActor*>(this)->actor_wait_state = 0;
		static_cast<JoinedFutureActor*>(this)->ActorCallback< JoinedFutureActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< JoinedFutureActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("joinedFuture", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("joinedFuture", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< JoinedFutureActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("joinedFuture", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("joinedFuture", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< JoinedFutureActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("joinedFuture", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("joinedFuture", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> future;
															#line 13463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via joinedFuture()
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinedFutureActor final : public Actor<Reference<TaskFuture>>, public ActorCallback< JoinedFutureActor, 0, Void >, public FastAllocated<JoinedFutureActor>, public JoinedFutureActorState<JoinedFutureActor> {
															#line 13468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<JoinedFutureActor>::operator new;
	using FastAllocated<JoinedFutureActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<TaskFuture>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< JoinedFutureActor, 0, Void >;
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinedFutureActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 13479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Reference<TaskFuture>>(),
		   JoinedFutureActorState<JoinedFutureActor>(tr, taskBucket, taskFuture)
	{
		fdb_probe_actor_enter("joinedFuture", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("joinedFuture");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("joinedFuture", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< JoinedFutureActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Reference<TaskFuture>> joinedFuture( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture ) {
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Reference<TaskFuture>>(new JoinedFutureActor(tr, taskBucket, taskFuture));
															#line 13506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};

TaskFuture::TaskFuture() {}

TaskFuture::TaskFuture(const Reference<FutureBucket> bucket, Key k) : futureBucket(bucket), key(k) {
	if (k.size() == 0) {
		key = deterministicRandom()->randomUniqueID().toString();
	}

	prefix = futureBucket->prefix.get(key);
	blocks = prefix.get(LiteralStringRef("bl"));
	callbacks = prefix.get(LiteralStringRef("cb"));
}

TaskFuture::~TaskFuture() {}

void TaskFuture::addBlock(Reference<ReadYourWritesTransaction> tr, StringRef block_id) {
	tr->set(blocks.pack(block_id), LiteralStringRef(""));
}

Future<Void> TaskFuture::set(Reference<ReadYourWritesTransaction> tr, Reference<TaskBucket> taskBucket) {
	return TaskFutureImpl::set(tr, taskBucket, Reference<TaskFuture>::addRef(this));
}

Future<Void> TaskFuture::performAllActions(Reference<ReadYourWritesTransaction> tr, Reference<TaskBucket> taskBucket) {
	return TaskFutureImpl::performAllActions(tr, taskBucket, Reference<TaskFuture>::addRef(this));
}

Future<Void> TaskFuture::join(Reference<ReadYourWritesTransaction> tr,
                              Reference<TaskBucket> taskBucket,
                              std::vector<Reference<TaskFuture>> vectorFuture) {
	return TaskFutureImpl::join(tr, taskBucket, Reference<TaskFuture>::addRef(this), vectorFuture);
}

Future<bool> TaskFuture::isSet(Reference<ReadYourWritesTransaction> tr) {
	return TaskFutureImpl::isSet(tr, Reference<TaskFuture>::addRef(this));
}

Future<Void> TaskFuture::onSet(Reference<ReadYourWritesTransaction> tr,
                               Reference<TaskBucket> taskBucket,
                               Reference<Task> task) {
	return TaskFutureImpl::onSet(tr, taskBucket, Reference<TaskFuture>::addRef(this), task);
}

Future<Void> TaskFuture::onSetAddTask(Reference<ReadYourWritesTransaction> tr,
                                      Reference<TaskBucket> taskBucket,
                                      Reference<Task> task) {
	return TaskFutureImpl::onSetAddTask(tr, taskBucket, Reference<TaskFuture>::addRef(this), task);
}

Future<Void> TaskFuture::onSetAddTask(Reference<ReadYourWritesTransaction> tr,
                                      Reference<TaskBucket> taskBucket,
                                      Reference<Task> task,
                                      KeyRef validationKey) {
	return TaskFutureImpl::onSetAddTask(tr, taskBucket, Reference<TaskFuture>::addRef(this), task, validationKey);
}

Future<Void> TaskFuture::onSetAddTask(Reference<ReadYourWritesTransaction> tr,
                                      Reference<TaskBucket> taskBucket,
                                      Reference<Task> task,
                                      KeyRef validationKey,
                                      KeyRef validationValue) {
	return TaskFutureImpl::onSetAddTask(
	    tr, taskBucket, Reference<TaskFuture>::addRef(this), task, validationKey, validationValue);
}

Future<Reference<TaskFuture>> TaskFuture::joinedFuture(Reference<ReadYourWritesTransaction> tr,
                                                       Reference<TaskBucket> taskBucket) {
	return TaskFutureImpl::joinedFuture(tr, taskBucket, Reference<TaskFuture>::addRef(this));
}

															#line 13581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
namespace {
// This generated class is to be used only via getCompletionKey()
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetCompletionKeyActor>
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetCompletionKeyActorState {
															#line 13588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetCompletionKeyActorState(TaskCompletionKey* const& self,Future<Reference<TaskFuture>> const& f) 
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : self(self),
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   f(f)
															#line 13597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
	{
		fdb_probe_actor_create("getCompletionKey", reinterpret_cast<unsigned long>(this));

	}
	~GetCompletionKeyActorState() 
	{
		fdb_probe_actor_destroy("getCompletionKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<TaskFuture>> __when_expr_0 = f;
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetCompletionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetCompletionKeyActor*>(this)->actor_wait_state = 1;
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >*>(static_cast<GetCompletionKeyActor*>(this)));
															#line 13619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		this->~GetCompletionKeyActorState();
		static_cast<GetCompletionKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<TaskFuture> const& taskFuture,int loopDepth) 
	{
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->joinFuture.clear();
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->key = taskFuture->key;
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetCompletionKeyActor*>(this)->SAV<Key>::futures) { (void)(self->key.get()); this->~GetCompletionKeyActorState(); static_cast<GetCompletionKeyActor*>(this)->destroy(); return 0; }
															#line 13646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetCompletionKeyActor*>(this)->SAV< Key >::value()) Key(self->key.get());
		this->~GetCompletionKeyActorState();
		static_cast<GetCompletionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<TaskFuture> && taskFuture,int loopDepth) 
	{
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->joinFuture.clear();
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->key = taskFuture->key;
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetCompletionKeyActor*>(this)->SAV<Key>::futures) { (void)(self->key.get()); this->~GetCompletionKeyActorState(); static_cast<GetCompletionKeyActor*>(this)->destroy(); return 0; }
															#line 13662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetCompletionKeyActor*>(this)->SAV< Key >::value()) Key(self->key.get());
		this->~GetCompletionKeyActorState();
		static_cast<GetCompletionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<TaskFuture> const& taskFuture,int loopDepth) 
	{
		loopDepth = a_body1cont1(taskFuture, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<TaskFuture> && taskFuture,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(taskFuture), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetCompletionKeyActor*>(this)->actor_wait_state > 0) static_cast<GetCompletionKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetCompletionKeyActor*>(this)->ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >::remove();

	}
	void a_callback_fire(ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >*,Reference<TaskFuture> const& value) 
	{
		fdb_probe_actor_enter("getCompletionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getCompletionKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >*,Reference<TaskFuture> && value) 
	{
		fdb_probe_actor_enter("getCompletionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getCompletionKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >*,Error err) 
	{
		fdb_probe_actor_enter("getCompletionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getCompletionKey", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskCompletionKey* self;
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Future<Reference<TaskFuture>> f;
															#line 13737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getCompletionKey()
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetCompletionKeyActor final : public Actor<Key>, public ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >, public FastAllocated<GetCompletionKeyActor>, public GetCompletionKeyActorState<GetCompletionKeyActor> {
															#line 13742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetCompletionKeyActor>::operator new;
	using FastAllocated<GetCompletionKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Key>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >;
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetCompletionKeyActor(TaskCompletionKey* const& self,Future<Reference<TaskFuture>> const& f) 
															#line 13753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		 : Actor<Key>(),
		   GetCompletionKeyActorState<GetCompletionKeyActor>(self, f)
	{
		fdb_probe_actor_enter("getCompletionKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getCompletionKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getCompletionKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] Future<Key> getCompletionKey( TaskCompletionKey* const& self, Future<Reference<TaskFuture>> const& f ) {
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Key>(new GetCompletionKeyActor(self, f));
															#line 13781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

Future<Key> TaskCompletionKey::get(Reference<ReadYourWritesTransaction> tr, Reference<TaskBucket> taskBucket) {
	ASSERT(key.present() == (joinFuture.getPtr() == nullptr));
	return key.present() ? key.get() : getCompletionKey(this, joinFuture->joinedFuture(tr, taskBucket));
}
