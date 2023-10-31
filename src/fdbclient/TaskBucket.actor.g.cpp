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
#include "fdbclient/FDBTypes.h"
#include "fdbclient/ReadYourWrites.h"
#include "flow/actorcompiler.h" // has to be last include

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

																#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via _finish()
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class _finishActor>
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _finishActorState {
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_finishActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   future(futureBucket->unpack(task->params[Task::reservedTaskParamKeyFuture]))
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			futureBucket->setOptions(tr);
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->clear(future->blocks.pack(task->params[Task::reservedTaskParamKeyBlockID]));
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = future->isSet(tr);
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_finishActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_finishActor*>(this)->actor_wait_state = 1;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _finishActor, 0, bool >*>(static_cast<_finishActor*>(this)));
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = future->performAllActions(tr, taskBucket);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_finishActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<_finishActor*>(this)->actor_wait_state = 2;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _finishActor, 1, Void >*>(static_cast<_finishActor*>(this)));
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = future->performAllActions(tr, taskBucket);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_finishActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<_finishActor*>(this)->actor_wait_state = 2;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _finishActor, 1, Void >*>(static_cast<_finishActor*>(this)));
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<_finishActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_finishActorState(); static_cast<_finishActor*>(this)->destroy(); return 0; }
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> future;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via _finish()
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _finishActor final : public Actor<Void>, public ActorCallback< _finishActor, 0, bool >, public ActorCallback< _finishActor, 1, Void >, public FastAllocated<_finishActor>, public _finishActorState<_finishActor> {
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<_finishActor>::operator new;
	using FastAllocated<_finishActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _finishActor, 0, bool >;
friend struct ActorCallback< _finishActor, 1, Void >;
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_finishActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> _finish( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, Reference<Task> const& task ) {
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new _finishActor(tr, taskBucket, futureBucket, task));
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};
StringRef UnblockFutureTaskFunc::name = "UnblockFuture"_sr;
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
StringRef AddTaskFunc::name = "AddTask"_sr;
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
StringRef IdleTaskFunc::name = "idle"_sr;
REGISTER_TASKFUNC(IdleTaskFunc);

Key Task::reservedTaskParamKeyType = "type"_sr;
Key Task::reservedTaskParamKeyAddTask = "_add_task"_sr;
Key Task::reservedTaskParamKeyDone = "done"_sr;
Key Task::reservedTaskParamKeyPriority = "priority"_sr;
Key Task::reservedTaskParamKeyFuture = "future"_sr;
Key Task::reservedTaskParamKeyBlockID = "blockid"_sr;
Key Task::reservedTaskParamKeyVersion = "version"_sr;
Key Task::reservedTaskParamValidKey = "_validkey"_sr;
Key Task::reservedTaskParamValidValue = "_validvalue"_sr;

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
																#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getTaskKey()
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetTaskKeyActor>
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskKeyActorState {
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskKeyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,int const& priority = 0) 
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   priority(priority)
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Standalone<StringRef> uid = StringRef(deterministicRandom()->randomUniqueID().toString());
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			space = taskBucket->getAvailableSpace(priority);
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				StrictFuture<RangeResult> __when_expr_0 = tr->getRange(KeyRangeRef(space.key(), space.pack(uid)), 1, Snapshot::True, Reverse::True);
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 1;
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTaskKeyActor, 0, RangeResult >*>(static_cast<GetTaskKeyActor*>(this)));
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_1 = tr->getRange(KeyRangeRef(space.key(), space.pack(maxUIDKey)), 1, Snapshot::True, Reverse::True);
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 2;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetTaskKeyActor, 1, RangeResult >*>(static_cast<GetTaskKeyActor*>(this)));
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2(RangeResult const& value,int loopDepth) 
	{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!value.empty())
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(value[0].key)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(value[0].key));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(RangeResult && value,int loopDepth) 
	{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!value.empty())
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(value[0].key)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(value[0].key));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult const& value,int loopDepth) 
	{
		loopDepth = a_body1cont2(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && value,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state > 0) static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetTaskKeyActor*>(this)->ActorCallback< GetTaskKeyActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 0, RangeResult >*,RangeResult const& value) 
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
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 0, RangeResult >*,RangeResult && value) 
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
	void a_callback_error(ActorCallback< GetTaskKeyActor, 0, RangeResult >*,Error err) 
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
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>()); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>());
		this->~GetTaskKeyActorState();
		static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(RangeResult const& value,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!value.empty())
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(value[0].key)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(value[0].key));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(RangeResult && value,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!value.empty())
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskKeyActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>(value[0].key)); this->~GetTaskKeyActorState(); static_cast<GetTaskKeyActor*>(this)->destroy(); return 0; }
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskKeyActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>(value[0].key));
			this->~GetTaskKeyActorState();
			static_cast<GetTaskKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& value,int loopDepth) 
	{
		loopDepth = a_body1cont6(value, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && value,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetTaskKeyActor*>(this)->actor_wait_state > 0) static_cast<GetTaskKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetTaskKeyActor*>(this)->ActorCallback< GetTaskKeyActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 1, RangeResult >*,RangeResult const& value) 
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
	void a_callback_fire(ActorCallback< GetTaskKeyActor, 1, RangeResult >*,RangeResult && value) 
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
	void a_callback_error(ActorCallback< GetTaskKeyActor, 1, RangeResult >*,Error err) 
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
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int priority;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace space;
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getTaskKey()
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskKeyActor final : public Actor<Optional<Key>>, public ActorCallback< GetTaskKeyActor, 0, RangeResult >, public ActorCallback< GetTaskKeyActor, 1, RangeResult >, public FastAllocated<GetTaskKeyActor>, public GetTaskKeyActorState<GetTaskKeyActor> {
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetTaskKeyActor>::operator new;
	using FastAllocated<GetTaskKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Key>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTaskKeyActor, 0, RangeResult >;
friend struct ActorCallback< GetTaskKeyActor, 1, RangeResult >;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskKeyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,int const& priority = 0) 
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
		case 1: this->a_callback_error((ActorCallback< GetTaskKeyActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetTaskKeyActor, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Optional<Key>> getTaskKey( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, int const& priority = 0 ) {
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Optional<Key>>(new GetTaskKeyActor(tr, taskBucket, priority));
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getOne()
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetOneActor>
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetOneActorState {
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetOneActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (taskBucket->priority_batch)
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->setOption(FDBTransactionOptions::PRIORITY_BATCH);
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (deterministicRandom()->random01() < CLIENT_KNOBS->TASKBUCKET_CHECK_TIMEOUT_CHANCE)
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				StrictFuture<bool> __when_expr_0 = requeueTimedOutTasks(tr, taskBucket);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetOneActor*>(this)->actor_wait_state = 1;
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 0, bool >*>(static_cast<GetOneActor*>(this)));
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskKeyFutures = std::vector<Future<Optional<Key>>>(CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY + 1);
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		pri = int();
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(pri = CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;pri >= 0;--pri) {
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKeyFutures[pri] = getTaskKey(tr, taskBucket, pri);
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskKey = Optional<Key>();
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		availableSpace = Subspace();
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		pri = CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool const& anyTimeouts,int loopDepth) 
	{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		CODE_PROBE(anyTimeouts, "Found a task that timed out");
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool && anyTimeouts,int loopDepth) 
	{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		CODE_PROBE(anyTimeouts, "Found a task that timed out");
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!taskKey.present())
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_2 = requeueTimedOutTasks(tr, taskBucket);
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 3;
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 2, bool >*>(static_cast<GetOneActor*>(this)));
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(pri >= 0))
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (taskKey.present())
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKeyFutures[pri].cancel();
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}
		else
		{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Key>> __when_expr_1 = taskKeyFutures[pri];
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 2;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 1, Optional<Key> >*>(static_cast<GetOneActor*>(this)));
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		--pri;
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Optional<Key> const& key,int loopDepth) 
	{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (key.present())
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKey = key;
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSpace = taskBucket->getAvailableSpace(pri);
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Optional<Key> && key,int loopDepth) 
	{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (key.present())
															#line 1110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskKey = key;
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSpace = taskBucket->getAvailableSpace(pri);
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		t = availableSpace.unpack(taskKey.get());
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskUID = t.getString(0);
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskAvailableSpace = availableSpace.get(taskUID);
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task = Reference<Task>(new Task());
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->key = taskUID;
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_4 = tr->getRange(taskAvailableSpace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont7when1(__when_expr_4.get(), loopDepth); };
		static_cast<GetOneActor*>(this)->actor_wait_state = 5;
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 4, RangeResult >*>(static_cast<GetOneActor*>(this)));
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont8(bool const& anyTimeouts,int loopDepth) 
	{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (anyTimeouts)
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Try to get one task from timeouts subspace");
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<Task>> __when_expr_3 = getOne(tr, taskBucket);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont8when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 4;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 3, Reference<Task> >*>(static_cast<GetOneActor*>(this)));
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (anyTimeouts)
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Try to get one task from timeouts subspace");
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<Task>> __when_expr_3 = getOne(tr, taskBucket);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont8when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetOneActor*>(this)->actor_wait_state = 4;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 3, Reference<Task> >*>(static_cast<GetOneActor*>(this)));
															#line 1255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(Reference<Task>()); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(Reference<Task>());
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(Reference<Task> const& task,int loopDepth) 
	{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(task);
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(Reference<Task> && task,int loopDepth) 
	{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Version> __when_expr_5 = tr->getReadVersion();
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<GetOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont12when1(__when_expr_5.get(), loopDepth); };
		static_cast<GetOneActor*>(this)->actor_wait_state = 6;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< GetOneActor, 5, Version >*>(static_cast<GetOneActor*>(this)));
															#line 1438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7when1(RangeResult const& __values,int loopDepth) 
	{
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		values = __values;
															#line 1447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = version + (uint64_t)(taskBucket->timeout * (CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_OFFSET + CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_RANGE * deterministicRandom()->random01()));
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Subspace timeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(taskUID);
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& s : values ) {
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = taskAvailableSpace.unpack(s.key).getString(0);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[param] = s.value;
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(timeoutSpace.pack(param), s.value);
															#line 1524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskAvailableSpace.range());
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->set(taskBucket->active.key(), deterministicRandom()->randomUniqueID().toString());
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetOneActor*>(this)->SAV< Reference<Task> >::value()) Reference<Task>(std::move(task)); // state_var_RVO
		this->~GetOneActorState();
		static_cast<GetOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12cont1(Version && version,int loopDepth) 
	{
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = version + (uint64_t)(taskBucket->timeout * (CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_OFFSET + CLIENT_KNOBS->TASKBUCKET_TIMEOUT_JITTER_RANGE * deterministicRandom()->random01()));
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Subspace timeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(taskUID);
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& s : values ) {
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = taskAvailableSpace.unpack(s.key).getString(0);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[param] = s.value;
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(timeoutSpace.pack(param), s.value);
															#line 1554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskAvailableSpace.range());
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->set(taskBucket->active.key(), deterministicRandom()->randomUniqueID().toString());
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetOneActor*>(this)->SAV<Reference<Task>>::futures) { (void)(task); this->~GetOneActorState(); static_cast<GetOneActor*>(this)->destroy(); return 0; }
															#line 1562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<Optional<Key>>> taskKeyFutures;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int pri;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Optional<Key> taskKey;
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace availableSpace;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Tuple t;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Key taskUID;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace taskAvailableSpace;
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RangeResult values;
															#line 1655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getOne()
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetOneActor final : public Actor<Reference<Task>>, public ActorCallback< GetOneActor, 0, bool >, public ActorCallback< GetOneActor, 1, Optional<Key> >, public ActorCallback< GetOneActor, 2, bool >, public ActorCallback< GetOneActor, 3, Reference<Task> >, public ActorCallback< GetOneActor, 4, RangeResult >, public ActorCallback< GetOneActor, 5, Version >, public FastAllocated<GetOneActor>, public GetOneActorState<GetOneActor> {
															#line 1660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetOneActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Reference<Task>> getOne( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Reference<Task>>(new GetOneActor(tr, taskBucket));
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	// Verify that the user configured task verification key still has the user specified value
																#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via taskVerify()
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class TaskVerifyActor>
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActorState {
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActorState(Reference<TaskBucket> const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task) 
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tb(tb),
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(tr),
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task->params.find(Task::reservedTaskParamValidKey) == task->params.end())
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", "missing");
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
				this->~TaskVerifyActorState();
				static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task->params.find(Task::reservedTaskParamValidValue) == task->params.end())
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", "missing");
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
				this->~TaskVerifyActorState();
				static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tb->setOptions(tr);
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(task->params[Task::reservedTaskParamValidKey]);
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<TaskVerifyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TaskVerifyActor*>(this)->actor_wait_state = 1;
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TaskVerifyActor, 0, Optional<Value> >*>(static_cast<TaskVerifyActor*>(this)));
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!keyValue.present())
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", "missing");
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (keyValue.get().compare(StringRef(task->params[Task::reservedTaskParamValidValue])))
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyAbortedTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", keyValue.get());
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~TaskVerifyActorState();
		static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && keyValue,int loopDepth) 
	{
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!keyValue.present())
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyInvalidTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", "missing");
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (keyValue.get().compare(StringRef(task->params[Task::reservedTaskParamValidValue])))
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent("TaskBucketTaskVerifyAbortedTask") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ReservedTaskParamValidKey", task->params[Task::reservedTaskParamValidKey]) .detail("ReservedTaskParamValidValue", task->params[Task::reservedTaskParamValidValue]) .detail("KeyValue", keyValue.get());
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<TaskVerifyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~TaskVerifyActorState();
			static_cast<TaskVerifyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~TaskVerifyActorState(); static_cast<TaskVerifyActor*>(this)->destroy(); return 0; }
															#line 1874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> tb;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 1951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via taskVerify()
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActor final : public Actor<bool>, public ActorCallback< TaskVerifyActor, 0, Optional<Value> >, public FastAllocated<TaskVerifyActor>, public TaskVerifyActorState<TaskVerifyActor> {
															#line 1956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<TaskVerifyActor>::operator new;
	using FastAllocated<TaskVerifyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TaskVerifyActor, 0, Optional<Value> >;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActor(Reference<TaskBucket> const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task) 
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> taskVerify( Reference<TaskBucket> const& tb, Reference<ReadYourWritesTransaction> const& tr, Reference<Task> const& task ) {
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new TaskVerifyActor(tb, tr, task));
															#line 1994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 1999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via taskVerify()
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class TaskVerifyActor1>
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActor1State {
															#line 2005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActor1State(Reference<TaskBucket> const& tb,Database const& cx,Reference<Task> const& task) 
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tb(tb),
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   cx(cx),
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 2016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 2031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 2061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		try {
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = taskVerify(tb, tr, task);
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<TaskVerifyActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TaskVerifyActor1*>(this)->actor_wait_state = 1;
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TaskVerifyActor1, 0, bool >*>(static_cast<TaskVerifyActor1*>(this)));
															#line 2072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<TaskVerifyActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TaskVerifyActor1*>(this)->actor_wait_state = 2;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TaskVerifyActor1, 1, Void >*>(static_cast<TaskVerifyActor1*>(this)));
															#line 2101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor1*>(this)->SAV<bool>::futures) { (void)(verified); this->~TaskVerifyActor1State(); static_cast<TaskVerifyActor1*>(this)->destroy(); return 0; }
															#line 2116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<TaskVerifyActor1*>(this)->SAV< bool >::value()) bool(verified);
		this->~TaskVerifyActor1State();
		static_cast<TaskVerifyActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && verified,int loopDepth) 
	{
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<TaskVerifyActor1*>(this)->SAV<bool>::futures) { (void)(verified); this->~TaskVerifyActor1State(); static_cast<TaskVerifyActor1*>(this)->destroy(); return 0; }
															#line 2128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> tb;
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 2282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via taskVerify()
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class TaskVerifyActor1 final : public Actor<bool>, public ActorCallback< TaskVerifyActor1, 0, bool >, public ActorCallback< TaskVerifyActor1, 1, Void >, public FastAllocated<TaskVerifyActor1>, public TaskVerifyActor1State<TaskVerifyActor1> {
															#line 2287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<TaskVerifyActor1>::operator new;
	using FastAllocated<TaskVerifyActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TaskVerifyActor1, 0, bool >;
friend struct ActorCallback< TaskVerifyActor1, 1, Void >;
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskVerifyActor1(Reference<TaskBucket> const& tb,Database const& cx,Reference<Task> const& task) 
															#line 2299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> taskVerify( Reference<TaskBucket> const& tb, Database const& cx, Reference<Task> const& task ) {
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new TaskVerifyActor1(tb, cx, task));
															#line 2327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via finishTaskRun()
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class FinishTaskRunActor>
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class FinishTaskRunActorState {
															#line 2338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	FinishTaskRunActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task,Reference<TaskFuncBase> const& taskFunc,VerifyTask const& verifyTask) 
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFunc(taskFunc),
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   verifyTask(verifyTask)
															#line 2355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = taskBucket->isFinished(tr, task);
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 1;
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 0, bool >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isFinished)
															#line 2400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<FinishTaskRunActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FinishTaskRunActorState(); static_cast<FinishTaskRunActor*>(this)->destroy(); return 0; }
															#line 2404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<FinishTaskRunActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FinishTaskRunActorState();
			static_cast<FinishTaskRunActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = true;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (verifyTask)
															#line 2414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskVerify(taskBucket, tr, task);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 2;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 1, bool >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isFinished)
															#line 2439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<FinishTaskRunActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FinishTaskRunActorState(); static_cast<FinishTaskRunActor*>(this)->destroy(); return 0; }
															#line 2443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<FinishTaskRunActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FinishTaskRunActorState();
			static_cast<FinishTaskRunActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = true;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (verifyTask)
															#line 2453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskVerify(taskBucket, tr, task);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 2;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 1, bool >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validTask)
															#line 2541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_2 = taskBucket->finish(tr, task);
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 3;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 2, Void >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_3 = taskFunc->finish(tr, taskBucket, futureBucket, task);
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<FinishTaskRunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont2when2(__when_expr_3.get(), loopDepth); };
			static_cast<FinishTaskRunActor*>(this)->actor_wait_state = 4;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FinishTaskRunActor, 3, Void >*>(static_cast<FinishTaskRunActor*>(this)));
															#line 2566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont4(bool const& _validTask,int loopDepth) 
	{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = _validTask;
															#line 2576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(bool && _validTask,int loopDepth) 
	{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		validTask = _validTask;
															#line 2585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<FinishTaskRunActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FinishTaskRunActorState(); static_cast<FinishTaskRunActor*>(this)->destroy(); return 0; }
															#line 2657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuncBase> taskFunc;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	VerifyTask verifyTask;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	bool validTask;
															#line 2829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via finishTaskRun()
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class FinishTaskRunActor final : public Actor<Void>, public ActorCallback< FinishTaskRunActor, 0, bool >, public ActorCallback< FinishTaskRunActor, 1, bool >, public ActorCallback< FinishTaskRunActor, 2, Void >, public ActorCallback< FinishTaskRunActor, 3, Void >, public FastAllocated<FinishTaskRunActor>, public FinishTaskRunActorState<FinishTaskRunActor> {
															#line 2834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	FinishTaskRunActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task,Reference<TaskFuncBase> const& taskFunc,VerifyTask const& verifyTask) 
															#line 2848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> finishTaskRun( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, Reference<Task> const& task, Reference<TaskFuncBase> const& taskFunc, VerifyTask const& verifyTask ) {
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new FinishTaskRunActor(tr, taskBucket, futureBucket, task, taskFunc, verifyTask));
															#line 2878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 2883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via doOne()
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DoOneActor>
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoOneActorState {
															#line 2889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoOneActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket) 
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket)
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<Task>> __when_expr_0 = taskBucket->getOne(cx);
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoOneActor*>(this)->actor_wait_state = 1;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoOneActor, 0, Reference<Task> >*>(static_cast<DoOneActor*>(this)));
															#line 2922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<bool> __when_expr_1 = taskBucket->doTask(cx, futureBucket, task);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoOneActor*>(this)->actor_wait_state = 2;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoOneActor, 1, bool >*>(static_cast<DoOneActor*>(this)));
															#line 2952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<Task> const& __task,int loopDepth) 
	{
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task = __task;
															#line 2961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoOneActor*>(this)->SAV<bool>::futures) { (void)(result); this->~DoOneActorState(); static_cast<DoOneActor*>(this)->destroy(); return 0; }
															#line 3028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoOneActor*>(this)->SAV< bool >::value()) bool(result);
		this->~DoOneActorState();
		static_cast<DoOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(bool && result,int loopDepth) 
	{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoOneActor*>(this)->SAV<bool>::futures) { (void)(result); this->~DoOneActorState(); static_cast<DoOneActor*>(this)->destroy(); return 0; }
															#line 3040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 3119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via doOne()
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoOneActor final : public Actor<bool>, public ActorCallback< DoOneActor, 0, Reference<Task> >, public ActorCallback< DoOneActor, 1, bool >, public FastAllocated<DoOneActor>, public DoOneActorState<DoOneActor> {
															#line 3124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DoOneActor>::operator new;
	using FastAllocated<DoOneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoOneActor, 0, Reference<Task> >;
friend struct ActorCallback< DoOneActor, 1, bool >;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoOneActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket) 
															#line 3136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> doOne( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket ) {
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new DoOneActor(cx, taskBucket, futureBucket));
															#line 3164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 3169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via extendTimeoutRepeatedly()
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class ExtendTimeoutRepeatedlyActor>
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutRepeatedlyActorState {
															#line 3175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutRepeatedlyActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx)),
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   start(now())
															#line 3190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Version> __when_expr_0 = runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) { taskBucket->setOptions(tr); return map(tr->getReadVersion(), [=](Version v) { return v; }); });
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 1;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 3235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Version const& __versionNow,int loopDepth) 
	{
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		versionNow = __versionNow;
															#line 3244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		releaser = FlowLock::Releaser();
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0.8 * (BUGGIFY ? (2 * deterministicRandom()->random01()) : 1.0) * (double)(task->timeoutVersion - (uint64_t)versionNow) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 2;
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (now() - start > 300)
															#line 3336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarnAlways, "TaskBucketLongExtend") .detail("Duration", now() - start) .detail("TaskUID", task->key) .detail("TaskType", task->params[Task::reservedTaskParamKeyType]) .detail("Priority", task->getPriority());
															#line 3340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = task->extendMutex.take();
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 3;
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (now() - start > 300)
															#line 3360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarnAlways, "TaskBucketLongExtend") .detail("Duration", now() - start) .detail("TaskUID", task->key) .detail("TaskType", task->params[Task::reservedTaskParamKeyType]) .detail("Priority", task->getPriority());
															#line 3364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = task->extendMutex.take();
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 3;
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		releaser = FlowLock::Releaser(task->extendMutex, 1);
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 3449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		releaser = FlowLock::Releaser(task->extendMutex, 1);
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 3460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->reset();
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Version> __when_expr_3 = taskBucket->extendTimeout(tr, task, UpdateParams::False);
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1loopBody1cont2loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1loopBody1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 4;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_5 = tr->onError(e);
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 3594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 2)); else return a_body1cont1loopBody1cont2loopBody1Catch1when1(__when_expr_5.get(), loopDepth); };
			static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 6;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_4 = tr->commit();
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1loopBody1cont2loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont1loopBody1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ExtendTimeoutRepeatedlyActor*>(this)->actor_wait_state = 5;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >*>(static_cast<ExtendTimeoutRepeatedlyActor*>(this)));
															#line 3621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1when1(Version const& __newTimeout,int loopDepth) 
	{
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		newTimeout = __newTimeout;
															#line 3630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = newTimeout;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		versionNow = tr->getCommittedVersion();
															#line 3699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		return a_body1cont1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->timeoutVersion = newTimeout;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		versionNow = tr->getCommittedVersion();
															#line 3710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	double start;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Version versionNow;
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	FlowLock::Releaser releaser;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Version newTimeout;
															#line 3869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via extendTimeoutRepeatedly()
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutRepeatedlyActor final : public Actor<Void>, public ActorCallback< ExtendTimeoutRepeatedlyActor, 0, Version >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 1, Void >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 2, Void >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 3, Version >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 4, Void >, public ActorCallback< ExtendTimeoutRepeatedlyActor, 5, Void >, public FastAllocated<ExtendTimeoutRepeatedlyActor>, public ExtendTimeoutRepeatedlyActorState<ExtendTimeoutRepeatedlyActor> {
															#line 3874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutRepeatedlyActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 3890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> extendTimeoutRepeatedly( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<Task> const& task ) {
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new ExtendTimeoutRepeatedlyActor(cx, taskBucket, task));
															#line 3922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 3927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via doTask()
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DoTaskActor>
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoTaskActorState {
															#line 3933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoTaskActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFunc(),
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   verifyTask(false)
															#line 3950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!task || !TaskFuncBase::isValidTask(task))
															#line 3965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(false); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 3969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(false);
				this->~DoTaskActorState();
				static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			try {
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				taskFunc = TaskFuncBase::create(task->params[Task::reservedTaskParamKeyType]);
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskFunc)
															#line 3980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					verifyTask.set(task->params.find(Task::reservedTaskParamValidKey) != task->params.end());
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					if (verifyTask)
															#line 3986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
					{
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
						;
															#line 3990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(true); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 4029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(true);
		this->~DoTaskActorState();
		static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarn, "TaskBucketExecuteFailure") .error(e) .detail("TaskUID", task->key) .detail("TaskType", task->params[Task::reservedTaskParamKeyType].printable()) .detail("Priority", task->getPriority());
															#line 4042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			try {
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				StrictFuture<Void> __when_expr_8 = taskFunc->handleError(cx, task, e);
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2Catch1(actor_cancelled(), loopDepth);
															#line 4048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch2Catch1(__when_expr_8.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_8.get(), loopDepth); };
				static_cast<DoTaskActor*>(this)->actor_wait_state = 9;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 8, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_5 = taskFunc->execute(cx, taskBucket, futureBucket, task) || extendTimeoutRepeatedly(cx, taskBucket, task);
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch2(__when_expr_5.getError(), loopDepth); else return a_body1cont4when1(__when_expr_5.get(), loopDepth); };
		static_cast<DoTaskActor*>(this)->actor_wait_state = 6;
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 5, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		taskBucket->setOptions(tr);
															#line 4111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		try {
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = taskVerify(taskBucket, tr, task);
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 1;
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 0, bool >*>(static_cast<DoTaskActor*>(this)));
															#line 4122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 5;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 4, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validTask)
															#line 4179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskBucket->isFinished(tr, task);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 2;
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 1, bool >*>(static_cast<DoTaskActor*>(this)));
															#line 4190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validTask)
															#line 4204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_1 = taskBucket->isFinished(tr, task);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 2;
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 1, bool >*>(static_cast<DoTaskActor*>(this)));
															#line 4215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!isFinished)
															#line 4298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_2 = taskBucket->finish(tr, task);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 3;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 2, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!isFinished)
															#line 4323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_2 = taskBucket->finish(tr, task);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 3;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 2, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<DoTaskActor*>(this)->actor_wait_state = 4;
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 3, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(true); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 4502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DoTaskActor*>(this)->SAV< bool >::value()) bool(true);
		this->~DoTaskActorState();
		static_cast<DoTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DoTaskActor*>(this)->SAV<bool>::futures) { (void)(true); this->~DoTaskActorState(); static_cast<DoTaskActor*>(this)->destroy(); return 0; }
															#line 4514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (BUGGIFY)
															#line 4664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delay(10.0);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch2(__when_expr_6.getError(), loopDepth); else return a_body1cont6when1(__when_expr_6.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 7;
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 6, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (BUGGIFY)
															#line 4689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delay(10.0);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch2(__when_expr_6.getError(), loopDepth); else return a_body1cont6when1(__when_expr_6.get(), loopDepth); };
			static_cast<DoTaskActor*>(this)->actor_wait_state = 7;
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 6, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_7 = runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) { return finishTaskRun(tr, taskBucket, futureBucket, task, taskFunc, verifyTask); });
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DoTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch2(__when_expr_7.getError(), loopDepth); else return a_body1cont7when1(__when_expr_7.get(), loopDepth); };
		static_cast<DoTaskActor*>(this)->actor_wait_state = 8;
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< DoTaskActor, 7, Void >*>(static_cast<DoTaskActor*>(this)));
															#line 4784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevWarn, "TaskBucketExecuteFailureLogErrorFailed") .error(e) .detail("TaskUID", task->key.printable()) .detail("TaskType", task->params[Task::reservedTaskParamKeyType].printable()) .detail("Priority", task->getPriority());
															#line 4963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuncBase> taskFunc;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	VerifyTask verifyTask;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 5076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via doTask()
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DoTaskActor final : public Actor<bool>, public ActorCallback< DoTaskActor, 0, bool >, public ActorCallback< DoTaskActor, 1, bool >, public ActorCallback< DoTaskActor, 2, Void >, public ActorCallback< DoTaskActor, 3, Void >, public ActorCallback< DoTaskActor, 4, Void >, public ActorCallback< DoTaskActor, 5, Void >, public ActorCallback< DoTaskActor, 6, Void >, public ActorCallback< DoTaskActor, 7, Void >, public ActorCallback< DoTaskActor, 8, Void >, public FastAllocated<DoTaskActor>, public DoTaskActorState<DoTaskActor> {
															#line 5081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DoTaskActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,Reference<Task> const& task) 
															#line 5100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> doTask( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, Reference<Task> const& task ) {
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new DoTaskActor(cx, taskBucket, futureBucket, task));
															#line 5135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 5140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via dispatch()
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DispatchActor>
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DispatchActorState {
															#line 5146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DispatchActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   pollDelay(pollDelay),
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   maxConcurrentTasks(maxConcurrentTasks),
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tasks(maxConcurrentTasks)
															#line 5163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& f : tasks ) {
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				f = Never();
															#line 5180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSlots = std::vector<int>();
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			availableSlots.reserve(tasks.size());
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int i = 0;i < tasks.size();++i) {
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.push_back(i);
															#line 5190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getTasks = std::vector<Future<Reference<Task>>>();
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = 1;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 5198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++taskBucket->dispatchSlotChecksStarted;
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 5230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++taskBucket->dispatchSlotChecksComplete;
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Future<Void> w = ready(waitForAny(tasks));
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!availableSlots.empty())
															#line 5243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (*pollDelay > 600)
															#line 5247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				TraceEvent(SevWarnAlways, "TaskBucketLongPollDelay").suppressFor(1.0).detail("Delay", *pollDelay);
															#line 5251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			w = w || delay(*pollDelay * (0.9 + deterministicRandom()->random01() / 5));
															#line 5255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = w;
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DispatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DispatchActor*>(this)->actor_wait_state = 2;
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DispatchActor, 1, Void >*>(static_cast<DispatchActor*>(this)));
															#line 5266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(!availableSlots.empty()))
															#line 5282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		getTasks.clear();
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0, imax = std::min<unsigned int>(getBatchSize, availableSlots.size());i < imax;++i) {
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getTasks.push_back(taskBucket->getOne(cx));
															#line 5292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_0 = waitForAllReady(getTasks);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<DispatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 5298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DispatchActor*>(this)->actor_wait_state = 1;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DispatchActor, 0, Void >*>(static_cast<DispatchActor*>(this)));
															#line 5303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		bool done = false;
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < getTasks.size();++i) {
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (getTasks[i].isError())
															#line 5329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchErrors;
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				continue;
			}
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Reference<Task> task = getTasks[i].get();
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task)
															#line 5342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchDoTasks;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				int slot = availableSlots.back();
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.pop_back();
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[slot] = taskBucket->doTask(cx, futureBucket, task);
															#line 5352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			else
			{
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchEmptyTasks;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (done)
															#line 5365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = 1;
															#line 5369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		else
		{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = std::min<unsigned int>(getBatchSize * 2, maxConcurrentTasks);
															#line 5376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		bool done = false;
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < getTasks.size();++i) {
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (getTasks[i].isError())
															#line 5390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchErrors;
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				continue;
			}
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Reference<Task> task = getTasks[i].get();
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task)
															#line 5403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchDoTasks;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				int slot = availableSlots.back();
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.pop_back();
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[slot] = taskBucket->doTask(cx, futureBucket, task);
															#line 5413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			else
			{
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				++taskBucket->dispatchEmptyTasks;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				done = true;
															#line 5421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (done)
															#line 5426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = 1;
															#line 5430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		else
		{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			getBatchSize = std::min<unsigned int>(getBatchSize * 2, maxConcurrentTasks);
															#line 5437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < tasks.size();++i) {
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (tasks[i].isReady())
															#line 5512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.push_back(i);
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[i] = Never();
															#line 5518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for(int i = 0;i < tasks.size();++i) {
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (tasks[i].isReady())
															#line 5531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				availableSlots.push_back(i);
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tasks[i] = Never();
															#line 5537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::shared_ptr<double const> pollDelay;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int maxConcurrentTasks;
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<bool>> tasks;
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<int> availableSlots;
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<Reference<Task>>> getTasks;
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	unsigned int getBatchSize;
															#line 5625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via dispatch()
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DispatchActor final : public Actor<Void>, public ActorCallback< DispatchActor, 0, Void >, public ActorCallback< DispatchActor, 1, Void >, public FastAllocated<DispatchActor>, public DispatchActorState<DispatchActor> {
															#line 5630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DispatchActor>::operator new;
	using FastAllocated<DispatchActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DispatchActor, 0, Void >;
friend struct ActorCallback< DispatchActor, 1, Void >;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DispatchActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 5642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> dispatch( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, std::shared_ptr<double const> const& pollDelay, int const& maxConcurrentTasks ) {
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new DispatchActor(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks));
															#line 5670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 5675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via watchPaused()
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class WatchPausedActor>
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class WatchPausedActorState {
															#line 5681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	WatchPausedActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<AsyncVar<bool>> const& paused) 
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   paused(paused)
															#line 5692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 5707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 5737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		try {
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(taskBucket->pauseKey);
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WatchPausedActor*>(this)->actor_wait_state = 1;
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 0, Optional<Value> >*>(static_cast<WatchPausedActor*>(this)));
															#line 5750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr->onError(e);
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<WatchPausedActor*>(this)->actor_wait_state = 4;
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 3, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		paused->set(pausedVal.present());
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		watchPausedFuture = tr->watch(taskBucket->pauseKey);
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 2;
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 1, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && pausedVal,int loopDepth) 
	{
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		paused->set(pausedVal.present());
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		watchPausedFuture = tr->watch(taskBucket->pauseKey);
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 2;
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 1, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = watchPausedFuture;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 3;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 2, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_2 = watchPausedFuture;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<WatchPausedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchPausedActor*>(this)->actor_wait_state = 3;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchPausedActor, 2, Void >*>(static_cast<WatchPausedActor*>(this)));
															#line 5920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<AsyncVar<bool>> paused;
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Future<Void> watchPausedFuture;
															#line 6161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via watchPaused()
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class WatchPausedActor final : public Actor<Void>, public ActorCallback< WatchPausedActor, 0, Optional<Value> >, public ActorCallback< WatchPausedActor, 1, Void >, public ActorCallback< WatchPausedActor, 2, Void >, public ActorCallback< WatchPausedActor, 3, Void >, public FastAllocated<WatchPausedActor>, public WatchPausedActorState<WatchPausedActor> {
															#line 6166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	WatchPausedActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<AsyncVar<bool>> const& paused) 
															#line 6180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> watchPaused( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<AsyncVar<bool>> const& paused ) {
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new WatchPausedActor(cx, taskBucket, paused));
															#line 6210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 6215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via run()
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class RunActor>
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RunActorState {
															#line 6221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RunActorState(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket),
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   pollDelay(pollDelay),
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   maxConcurrentTasks(maxConcurrentTasks),
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   paused(makeReference<AsyncVar<bool>>(true)),
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   watchPausedFuture(watchPaused(cx, taskBucket, paused))
															#line 6240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->metricLogger = taskBucket->cc.traceCounters( "TaskBucketMetrics", taskBucket->dbgid, CLIENT_KNOBS->TASKBUCKET_LOGGING_DELAY);
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 6257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 6287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = dispatch(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks) || paused->onChange() || watchPausedFuture;
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunActor*>(this)->actor_wait_state = 2;
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunActor, 1, Void >*>(static_cast<RunActor*>(this)));
															#line 6303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(paused->get()))
															#line 6319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_0 = paused->onChange() || watchPausedFuture;
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RunActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 6327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RunActor*>(this)->actor_wait_state = 1;
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunActor, 0, Void >*>(static_cast<RunActor*>(this)));
															#line 6332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::shared_ptr<double const> pollDelay;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int maxConcurrentTasks;
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<AsyncVar<bool>> paused;
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Future<Void> watchPausedFuture;
															#line 6514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via run()
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RunActor final : public Actor<Void>, public ActorCallback< RunActor, 0, Void >, public ActorCallback< RunActor, 1, Void >, public FastAllocated<RunActor>, public RunActorState<RunActor> {
															#line 6519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<RunActor>::operator new;
	using FastAllocated<RunActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunActor, 0, Void >;
friend struct ActorCallback< RunActor, 1, Void >;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RunActor(Database const& cx,Reference<TaskBucket> const& taskBucket,Reference<FutureBucket> const& futureBucket,std::shared_ptr<double const> const& pollDelay,int const& maxConcurrentTasks) 
															#line 6531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> run( Database const& cx, Reference<TaskBucket> const& taskBucket, Reference<FutureBucket> const& futureBucket, std::shared_ptr<double const> const& pollDelay, int const& maxConcurrentTasks ) {
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new RunActor(cx, taskBucket, futureBucket, pollDelay, maxConcurrentTasks));
															#line 6559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	static Future<Standalone<StringRef>> addIdle(Reference<ReadYourWritesTransaction> tr,
	                                             Reference<TaskBucket> taskBucket) {
		taskBucket->setOptions(tr);

		auto newTask = makeReference<Task>(IdleTaskFunc::name, IdleTaskFunc::version);
		return taskBucket->addTask(tr, newTask);
	}

	static Future<Standalone<StringRef>> addIdle(Database cx, Reference<TaskBucket> taskBucket) {
		return runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) { return addIdle(tr, taskBucket); });
	}

																#line 6576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isEmpty()
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsEmptyActor>
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActorState {
															#line 6582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 6591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			resultFutures = std::vector<Future<RangeResult>>();
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int pri = 0;pri <= CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;++pri) {
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				resultFutures.push_back(tr->getRange(taskBucket->getAvailableSpace(pri).range(), 1));
															#line 6612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = int();
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = 0;
															#line 6618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(taskBucket->timeouts.range(), 1);
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<IsEmptyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<IsEmptyActor*>(this)->actor_wait_state = 2;
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IsEmptyActor, 1, RangeResult >*>(static_cast<IsEmptyActor*>(this)));
															#line 6648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(i < resultFutures.size()))
															#line 6664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = resultFutures[i];
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<IsEmptyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<IsEmptyActor*>(this)->actor_wait_state = 1;
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsEmptyActor, 0, RangeResult >*>(static_cast<IsEmptyActor*>(this)));
															#line 6677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 6699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 6711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && results,int loopDepth) 
	{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 6720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 6732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 6804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsEmptyActorState();
		static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(RangeResult && values,int loopDepth) 
	{
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 6828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsEmptyActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsEmptyActorState();
			static_cast<IsEmptyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsEmptyActorState(); static_cast<IsEmptyActor*>(this)->destroy(); return 0; }
															#line 6840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<RangeResult>> resultFutures;
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int i;
															#line 6919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isEmpty()
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActor final : public Actor<bool>, public ActorCallback< IsEmptyActor, 0, RangeResult >, public ActorCallback< IsEmptyActor, 1, RangeResult >, public FastAllocated<IsEmptyActor>, public IsEmptyActorState<IsEmptyActor> {
															#line 6924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsEmptyActor>::operator new;
	using FastAllocated<IsEmptyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsEmptyActor, 0, RangeResult >;
friend struct ActorCallback< IsEmptyActor, 1, RangeResult >;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 6936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isEmpty( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsEmptyActor(tr, taskBucket));
															#line 6964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 6969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isBusy()
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsBusyActor>
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsBusyActorState {
															#line 6975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsBusyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 6984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			resultFutures = std::vector<Future<RangeResult>>();
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int pri = 0;pri <= CLIENT_KNOBS->TASKBUCKET_MAX_PRIORITY;++pri) {
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				resultFutures.push_back(tr->getRange(taskBucket->getAvailableSpace(pri).range(), 1));
															#line 7005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = int();
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			i = 0;
															#line 7011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsBusyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsBusyActorState(); static_cast<IsBusyActor*>(this)->destroy(); return 0; }
															#line 7034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(i < resultFutures.size()))
															#line 7053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = resultFutures[i];
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<IsBusyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<IsBusyActor*>(this)->actor_wait_state = 1;
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsBusyActor, 0, RangeResult >*>(static_cast<IsBusyActor*>(this)));
															#line 7066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 7088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsBusyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsBusyActorState(); static_cast<IsBusyActor*>(this)->destroy(); return 0; }
															#line 7092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsBusyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~IsBusyActorState();
			static_cast<IsBusyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 7100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && results,int loopDepth) 
	{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (results.size() > 0)
															#line 7109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsBusyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsBusyActorState(); static_cast<IsBusyActor*>(this)->destroy(); return 0; }
															#line 7113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsBusyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~IsBusyActorState();
			static_cast<IsBusyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++i;
															#line 7121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Future<RangeResult>> resultFutures;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int i;
															#line 7197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isBusy()
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsBusyActor final : public Actor<bool>, public ActorCallback< IsBusyActor, 0, RangeResult >, public FastAllocated<IsBusyActor>, public IsBusyActorState<IsBusyActor> {
															#line 7202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsBusyActor>::operator new;
	using FastAllocated<IsBusyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsBusyActor, 0, RangeResult >;
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsBusyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 7213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isBusy( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsBusyActor(tr, taskBucket));
															#line 7240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	// Verify that the task's keys are still in the timeout space at the expected timeout prefix
																#line 7246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isFinished()
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsFinishedActor>
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsFinishedActorState {
															#line 7252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsFinishedActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 7263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple t = Tuple::makeTuple(task->timeoutVersion, task->key);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(taskBucket->timeouts.range(t), 1);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<IsFinishedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsFinishedActor*>(this)->actor_wait_state = 1;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsFinishedActor, 0, RangeResult >*>(static_cast<IsFinishedActor*>(this)));
															#line 7289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 7312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsFinishedActorState();
			static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsFinishedActorState();
		static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 7336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsFinishedActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsFinishedActorState();
			static_cast<IsFinishedActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsFinishedActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsFinishedActorState(); static_cast<IsFinishedActor*>(this)->destroy(); return 0; }
															#line 7348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 7425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isFinished()
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsFinishedActor final : public Actor<bool>, public ActorCallback< IsFinishedActor, 0, RangeResult >, public FastAllocated<IsFinishedActor>, public IsFinishedActorState<IsFinishedActor> {
															#line 7430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsFinishedActor>::operator new;
	using FastAllocated<IsFinishedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsFinishedActor, 0, RangeResult >;
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsFinishedActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task) 
															#line 7441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isFinished( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<Task> const& task ) {
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsFinishedActor(tr, taskBucket, task));
															#line 7468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 7473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getActiveKey()
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetActiveKeyActor>
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetActiveKeyActorState {
															#line 7479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetActiveKeyActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Optional<Value> const& startingValue) 
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   startingValue(startingValue)
															#line 7490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(taskBucket->active.key());
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetActiveKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetActiveKeyActor*>(this)->actor_wait_state = 1;
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetActiveKeyActor, 0, Optional<Value> >*>(static_cast<GetActiveKeyActor*>(this)));
															#line 7514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (new_value != startingValue)
															#line 7537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~GetActiveKeyActorState();
			static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~GetActiveKeyActorState();
		static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && new_value,int loopDepth) 
	{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (new_value != startingValue)
															#line 7561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetActiveKeyActor*>(this)->SAV< bool >::value()) bool(true);
			this->~GetActiveKeyActorState();
			static_cast<GetActiveKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetActiveKeyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~GetActiveKeyActorState(); static_cast<GetActiveKeyActor*>(this)->destroy(); return 0; }
															#line 7573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Optional<Value> startingValue;
															#line 7650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getActiveKey()
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetActiveKeyActor final : public Actor<bool>, public ActorCallback< GetActiveKeyActor, 0, Optional<Value> >, public FastAllocated<GetActiveKeyActor>, public GetActiveKeyActorState<GetActiveKeyActor> {
															#line 7655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetActiveKeyActor>::operator new;
	using FastAllocated<GetActiveKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetActiveKeyActor, 0, Optional<Value> >;
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetActiveKeyActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Optional<Value> const& startingValue) 
															#line 7666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> getActiveKey( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Optional<Value> const& startingValue ) {
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new GetActiveKeyActor(tr, taskBucket, startingValue));
															#line 7693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 7698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via checkActive()
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class CheckActiveActor>
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class CheckActiveActorState {
															#line 7704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	CheckActiveActorState(Database const& cx,Reference<TaskBucket> const& taskBucket) 
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : cx(cx),
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx)),
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   startingValue()
															#line 7717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			;
															#line 7732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		idx = 0;
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 7757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = isBusy(tr, taskBucket);
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 1;
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 0, bool >*>(static_cast<CheckActiveActor*>(this)));
															#line 7783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 5;
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 4, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 7825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!is_busy)
															#line 7840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(addIdle(tr, taskBucket));
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 2;
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 1, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 7851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!is_busy)
															#line 7865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(addIdle(tr, taskBucket));
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 2;
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 1, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 7876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Optional<Value>> __when_expr_2 = tr->get(taskBucket->active.key());
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 3;
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 2, Optional<Value> >*>(static_cast<CheckActiveActor*>(this)));
															#line 7960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		startingValue = val;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 4;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 3, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5(Optional<Value> && val,int loopDepth) 
	{
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		startingValue = val;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 4;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 3, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		CODE_PROBE(true, "checkActive return false");
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<CheckActiveActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckActiveActorState(); static_cast<CheckActiveActor*>(this)->destroy(); return 0; }
															#line 8295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!(idx < CLIENT_KNOBS->TASKBUCKET_CHECK_ACTIVE_AMOUNT))
															#line 8314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr = Reference<ReadYourWritesTransaction>(new ReadYourWritesTransaction(cx));
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		;
															#line 8322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		++idx;
															#line 8344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_5 = delay(CLIENT_KNOBS->TASKBUCKET_CHECK_ACTIVE_DELAY);
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1cont1loopBody1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont1loopBody1loopBody1when1(__when_expr_5.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 6;
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 5, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_7 = tr->onError(e);
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 8407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), std::max(0, loopDepth - 2)); else return a_body1cont1loopBody1loopBody1Catch1when1(__when_expr_7.get(), loopDepth); };
			static_cast<CheckActiveActor*>(this)->actor_wait_state = 8;
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 7, Void >*>(static_cast<CheckActiveActor*>(this)));
															#line 8412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<bool> __when_expr_6 = getActiveKey(tr, taskBucket, startingValue);
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1cont1loopBody1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont1loopBody1loopBody1cont2when1(__when_expr_6.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 7;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 6, bool >*>(static_cast<CheckActiveActor*>(this)));
															#line 8434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<bool> __when_expr_6 = getActiveKey(tr, taskBucket, startingValue);
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<CheckActiveActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1cont1loopBody1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont1loopBody1loopBody1cont2when1(__when_expr_6.get(), loopDepth); };
		static_cast<CheckActiveActor*>(this)->actor_wait_state = 7;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< CheckActiveActor, 6, bool >*>(static_cast<CheckActiveActor*>(this)));
															#line 8450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isActiveKey)
															#line 8522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "checkActive return true");
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<CheckActiveActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckActiveActorState(); static_cast<CheckActiveActor*>(this)->destroy(); return 0; }
															#line 8528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (isActiveKey)
															#line 8542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "checkActive return true");
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<CheckActiveActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckActiveActorState(); static_cast<CheckActiveActor*>(this)->destroy(); return 0; }
															#line 8548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Database cx;
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Optional<Value> startingValue;
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	int idx;
															#line 8706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via checkActive()
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class CheckActiveActor final : public Actor<bool>, public ActorCallback< CheckActiveActor, 0, bool >, public ActorCallback< CheckActiveActor, 1, Void >, public ActorCallback< CheckActiveActor, 2, Optional<Value> >, public ActorCallback< CheckActiveActor, 3, Void >, public ActorCallback< CheckActiveActor, 4, Void >, public ActorCallback< CheckActiveActor, 5, Void >, public ActorCallback< CheckActiveActor, 6, bool >, public ActorCallback< CheckActiveActor, 7, Void >, public FastAllocated<CheckActiveActor>, public CheckActiveActorState<CheckActiveActor> {
															#line 8711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	CheckActiveActor(Database const& cx,Reference<TaskBucket> const& taskBucket) 
															#line 8729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> checkActive( Database const& cx, Reference<TaskBucket> const& taskBucket ) {
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new CheckActiveActor(cx, taskBucket));
															#line 8763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 8768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via getTaskCount()
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetTaskCountActor>
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskCountActorState {
															#line 8774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskCountActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 8783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(taskBucket->prefix.pack("task_count"_sr));
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetTaskCountActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTaskCountActor*>(this)->actor_wait_state = 1;
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTaskCountActor, 0, Optional<Value> >*>(static_cast<GetTaskCountActor*>(this)));
															#line 8807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!val.present())
															#line 8830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(0); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(0);
			this->~GetTaskCountActorState();
			static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(val.get().size() == sizeof(int64_t));
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		int64_t intValue = 0;
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		memcpy(&intValue, val.get().begin(), val.get().size());
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(intValue); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(intValue);
		this->~GetTaskCountActorState();
		static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!val.present())
															#line 8860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(0); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<GetTaskCountActor*>(this)->SAV< int64_t >::value()) int64_t(0);
			this->~GetTaskCountActorState();
			static_cast<GetTaskCountActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(val.get().size() == sizeof(int64_t));
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		int64_t intValue = 0;
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		memcpy(&intValue, val.get().begin(), val.get().size());
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetTaskCountActor*>(this)->SAV<int64_t>::futures) { (void)(intValue); this->~GetTaskCountActorState(); static_cast<GetTaskCountActor*>(this)->destroy(); return 0; }
															#line 8878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 8953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getTaskCount()
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetTaskCountActor final : public Actor<int64_t>, public ActorCallback< GetTaskCountActor, 0, Optional<Value> >, public FastAllocated<GetTaskCountActor>, public GetTaskCountActorState<GetTaskCountActor> {
															#line 8958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetTaskCountActor>::operator new;
	using FastAllocated<GetTaskCountActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTaskCountActor, 0, Optional<Value> >;
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetTaskCountActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 8969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<int64_t> getTaskCount( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<int64_t>(new GetTaskCountActor(tr, taskBucket));
															#line 8996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

	// Looks for tasks that have timed out and returns them to be available tasks.
	// Returns True if any tasks were affected.
																#line 9003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via requeueTimedOutTasks()
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class RequeueTimedOutTasksActor>
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RequeueTimedOutTasksActorState {
															#line 9009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RequeueTimedOutTasksActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket)
															#line 9018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Looks for tasks that have timed out and returns them to be available tasks.");
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Version> __when_expr_0 = tr->getReadVersion();
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 1;
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RequeueTimedOutTasksActor, 0, Version >*>(static_cast<RequeueTimedOutTasksActor*>(this)));
															#line 9042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		range = KeyRange(KeyRangeRef(taskBucket->timeouts.get(0).range().begin, taskBucket->timeouts.get(end).range().end));
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(range, CLIENT_KNOBS->TASKBUCKET_MAX_TASK_KEYS);
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 2;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*>(static_cast<RequeueTimedOutTasksActor*>(this)));
															#line 9074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Version && end,int loopDepth) 
	{
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		range = KeyRange(KeyRangeRef(taskBucket->timeouts.get(0).range().begin, taskBucket->timeouts.get(end).range().end));
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(range, CLIENT_KNOBS->TASKBUCKET_MAX_TASK_KEYS);
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RequeueTimedOutTasksActor*>(this)->actor_wait_state = 2;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >*>(static_cast<RequeueTimedOutTasksActor*>(this)));
															#line 9092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Task task;
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Key lastKey;
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& iter : values ) {
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple t = taskBucket->timeouts.unpack(iter.key);
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key uid = t.getString(1);
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = t.getString(2);
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (uid != task.key)
															#line 9176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				for( auto& p : task.params ) {
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					tr->set(space.pack(p.key), p.value);
															#line 9184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.params.clear();
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.key = uid;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastKey = iter.key;
															#line 9192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task.params[param] = iter.value;
															#line 9196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!values.more)
															#line 9200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task.params ) {
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(space.pack(p.key), p.value);
															#line 9208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (values.size() > 0)
															#line 9212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->clear(range);
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
				this->~RequeueTimedOutTasksActorState();
				static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(false); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(false);
			this->~RequeueTimedOutTasksActorState();
			static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(lastKey != Key());
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(KeyRangeRef(range.begin, lastKey));
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
		this->~RequeueTimedOutTasksActorState();
		static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult && values,int loopDepth) 
	{
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Task task;
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		Key lastKey;
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& iter : values ) {
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple t = taskBucket->timeouts.unpack(iter.key);
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key uid = t.getString(1);
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key param = t.getString(2);
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (uid != task.key)
															#line 9262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				for( auto& p : task.params ) {
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					tr->set(space.pack(p.key), p.value);
															#line 9270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.params.clear();
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task.key = uid;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastKey = iter.key;
															#line 9278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task.params[param] = iter.value;
															#line 9282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!values.more)
															#line 9286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace space = taskBucket->getAvailableSpace(task.getPriority()).get(task.key);
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task.params ) {
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(space.pack(p.key), p.value);
															#line 9294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (values.size() > 0)
															#line 9298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->clear(range);
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(true);
				this->~RequeueTimedOutTasksActorState();
				static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(false); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<RequeueTimedOutTasksActor*>(this)->SAV< bool >::value()) bool(false);
			this->~RequeueTimedOutTasksActorState();
			static_cast<RequeueTimedOutTasksActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		ASSERT(lastKey != Key());
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(KeyRangeRef(range.begin, lastKey));
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<RequeueTimedOutTasksActor*>(this)->SAV<bool>::futures) { (void)(true); this->~RequeueTimedOutTasksActorState(); static_cast<RequeueTimedOutTasksActor*>(this)->destroy(); return 0; }
															#line 9324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	KeyRange range;
															#line 9401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via requeueTimedOutTasks()
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class RequeueTimedOutTasksActor final : public Actor<bool>, public ActorCallback< RequeueTimedOutTasksActor, 0, Version >, public ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >, public FastAllocated<RequeueTimedOutTasksActor>, public RequeueTimedOutTasksActorState<RequeueTimedOutTasksActor> {
															#line 9406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<RequeueTimedOutTasksActor>::operator new;
	using FastAllocated<RequeueTimedOutTasksActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RequeueTimedOutTasksActor, 0, Version >;
friend struct ActorCallback< RequeueTimedOutTasksActor, 1, RangeResult >;
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	RequeueTimedOutTasksActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket) 
															#line 9418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> requeueTimedOutTasks( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket ) {
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new RequeueTimedOutTasksActor(tr, taskBucket));
															#line 9446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 9451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via debugPrintRange()
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class DebugPrintRangeActor>
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DebugPrintRangeActorState {
															#line 9457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DebugPrintRangeActorState(Reference<ReadYourWritesTransaction> const& tr,Subspace const& subspace,Key const& msg) 
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   subspace(subspace),
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   msg(msg)
															#line 9468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(subspace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<DebugPrintRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DebugPrintRangeActor*>(this)->actor_wait_state = 1;
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DebugPrintRangeActor, 0, RangeResult >*>(static_cast<DebugPrintRangeActor*>(this)));
															#line 9494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TraceEvent("TaskBucketDebugPrintRange") .detail("Key", subspace.key()) .detail("Count", values.size()) .detail("Msg", msg);
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DebugPrintRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DebugPrintRangeActorState(); static_cast<DebugPrintRangeActor*>(this)->destroy(); return 0; }
															#line 9519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<DebugPrintRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DebugPrintRangeActorState();
		static_cast<DebugPrintRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TraceEvent("TaskBucketDebugPrintRange") .detail("Key", subspace.key()) .detail("Count", values.size()) .detail("Msg", msg);
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<DebugPrintRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DebugPrintRangeActorState(); static_cast<DebugPrintRangeActor*>(this)->destroy(); return 0; }
															#line 9533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace subspace;
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Key msg;
															#line 9610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via debugPrintRange()
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class DebugPrintRangeActor final : public Actor<Void>, public ActorCallback< DebugPrintRangeActor, 0, RangeResult >, public FastAllocated<DebugPrintRangeActor>, public DebugPrintRangeActorState<DebugPrintRangeActor> {
															#line 9615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<DebugPrintRangeActor>::operator new;
	using FastAllocated<DebugPrintRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DebugPrintRangeActor, 0, RangeResult >;
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	DebugPrintRangeActor(Reference<ReadYourWritesTransaction> const& tr,Subspace const& subspace,Key const& msg) 
															#line 9626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> debugPrintRange( Reference<ReadYourWritesTransaction> const& tr, Subspace const& subspace, Key const& msg ) {
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new DebugPrintRangeActor(tr, subspace, msg));
															#line 9653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 9658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via extendTimeout()
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class ExtendTimeoutActor>
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutActorState {
															#line 9664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task,UpdateParams const& updateParams,Version const& newTimeoutVersion) 
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   updateParams(updateParams),
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   newTimeoutVersion(newTimeoutVersion)
															#line 9679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskBucket->setOptions(tr);
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = taskBucket->keepRunning(tr, task);
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 1;
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 0, Void >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		oldTimeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(task->key);
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Version> __when_expr_1 = tr->getReadVersion();
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 2;
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 1, Version >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		oldTimeoutSpace = taskBucket->timeouts.get(task->timeoutVersion).get(task->key);
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Version> __when_expr_1 = tr->getReadVersion();
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 2;
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 1, Version >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion == invalidVersion)
															#line 9825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = version + taskBucket->timeout;
															#line 9829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		else
		{
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (newTimeoutVersion <= version)
															#line 9835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				newTimeoutVersion = version + 1;
															#line 9839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion <= task->timeoutVersion)
															#line 9844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = task->timeoutVersion + 1;
															#line 9848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		newTimeoutSpace = taskBucket->timeouts.get(newTimeoutVersion).get(task->key);
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addReadConflictRange(oldTimeoutSpace.range());
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addWriteConflictRange(newTimeoutSpace.range());
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (updateParams)
															#line 9858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Extended a task while updating parameters");
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task->params ) {
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(newTimeoutSpace.pack(p.key), p.value);
															#line 9866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont3(loopDepth);
		}
		else
		{
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Extended a task without updating parameters");
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = tr->getRange(oldTimeoutSpace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 3;
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 2, RangeResult >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2(Version && version,int loopDepth) 
	{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion == invalidVersion)
															#line 9893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = version + taskBucket->timeout;
															#line 9897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		else
		{
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (newTimeoutVersion <= version)
															#line 9903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				newTimeoutVersion = version + 1;
															#line 9907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
		}
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (newTimeoutVersion <= task->timeoutVersion)
															#line 9912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			newTimeoutVersion = task->timeoutVersion + 1;
															#line 9916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		newTimeoutSpace = taskBucket->timeouts.get(newTimeoutVersion).get(task->key);
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addReadConflictRange(oldTimeoutSpace.range());
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->addWriteConflictRange(newTimeoutSpace.range());
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (updateParams)
															#line 9926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Extended a task while updating parameters");
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& p : task->params ) {
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(newTimeoutSpace.pack(p.key), p.value);
															#line 9934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont3(loopDepth);
		}
		else
		{
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "Extended a task without updating parameters");
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = tr->getRange(oldTimeoutSpace.range(), CLIENT_KNOBS->TOO_MANY);
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ExtendTimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ExtendTimeoutActor*>(this)->actor_wait_state = 3;
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExtendTimeoutActor, 2, RangeResult >*>(static_cast<ExtendTimeoutActor*>(this)));
															#line 9951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(oldTimeoutSpace.range());
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<ExtendTimeoutActor*>(this)->SAV<Version>::futures) { (void)(newTimeoutVersion); this->~ExtendTimeoutActorState(); static_cast<ExtendTimeoutActor*>(this)->destroy(); return 0; }
															#line 10026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<ExtendTimeoutActor*>(this)->SAV< Version >::value()) Version(std::move(newTimeoutVersion)); // state_var_RVO
		this->~ExtendTimeoutActorState();
		static_cast<ExtendTimeoutActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(RangeResult const& params,int loopDepth) 
	{
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& kv : params ) {
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple paramKey = oldTimeoutSpace.unpack(kv.key);
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(newTimeoutSpace.pack(paramKey), kv.value);
															#line 10042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(RangeResult && params,int loopDepth) 
	{
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		for( auto& kv : params ) {
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Tuple paramKey = oldTimeoutSpace.unpack(kv.key);
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->set(newTimeoutSpace.pack(paramKey), kv.value);
															#line 10056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	UpdateParams updateParams;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Version newTimeoutVersion;
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace oldTimeoutSpace;
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Subspace newTimeoutSpace;
															#line 10139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via extendTimeout()
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ExtendTimeoutActor final : public Actor<Version>, public ActorCallback< ExtendTimeoutActor, 0, Void >, public ActorCallback< ExtendTimeoutActor, 1, Version >, public ActorCallback< ExtendTimeoutActor, 2, RangeResult >, public FastAllocated<ExtendTimeoutActor>, public ExtendTimeoutActorState<ExtendTimeoutActor> {
															#line 10144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ExtendTimeoutActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<Task> const& task,UpdateParams const& updateParams,Version const& newTimeoutVersion) 
															#line 10157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Version> extendTimeout( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<Task> const& task, UpdateParams const& updateParams, Version const& newTimeoutVersion ) {
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Version>(new ExtendTimeoutActor(tr, taskBucket, task, updateParams, newTimeoutVersion));
															#line 10186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};

TaskBucket::TaskBucket(const Subspace& subspace,
                       AccessSystemKeys sysAccess,
                       PriorityBatch priorityBatch,
                       LockAware lockAware)
  : dbgid(deterministicRandom()->randomUniqueID()), cc("TaskBucket", dbgid.toString()),
    dispatchSlotChecksStarted("DispatchSlotChecksStarted", cc), dispatchErrors("DispatchErrors", cc),
    dispatchDoTasks("DispatchDoTasks", cc), dispatchEmptyTasks("DispatchEmptyTasks", cc),
    dispatchSlotChecksComplete("DispatchSlotChecksComplete", cc), prefix(subspace), active(prefix.get("ac"_sr)),
    pauseKey(prefix.pack("pause"_sr)), available(prefix.get("av"_sr)), available_prioritized(prefix.get("avp"_sr)),
    timeouts(prefix.get("to"_sr)), timeout(CLIENT_KNOBS->TASKBUCKET_TIMEOUT_VERSIONS), system_access(sysAccess),
    priority_batch(priorityBatch), lockAware(lockAware) {}

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

	tr->atomicOp(prefix.pack("task_count"_sr), "\x01\x00\x00\x00\x00\x00\x00\x00"_sr, MutationRef::AddValue);

	return key;
}

void TaskBucket::setValidationCondition(Reference<Task> task, KeyRef vKey, KeyRef vValue) {
	task->params[Task::reservedTaskParamValidKey] = vKey;
	task->params[Task::reservedTaskParamValidValue] = vValue;
}

															#line 10255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
namespace {
// This generated class is to be used only via actorAddTask()
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class ActorAddTaskActor>
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ActorAddTaskActorState {
															#line 10262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ActorAddTaskActorState(TaskBucket* const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tb(tb),
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   tr(tr),
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   validationKey(validationKey)
															#line 10275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tb->setOptions(tr);
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(validationKey);
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<ActorAddTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ActorAddTaskActor*>(this)->actor_wait_state = 1;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorAddTaskActor, 0, Optional<Value> >*>(static_cast<ActorAddTaskActor*>(this)));
															#line 10299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 10322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 10328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TaskBucket::setValidationCondition(task, validationKey, validationValue.get());
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<ActorAddTaskActor*>(this)->SAV<Key>::futures) { (void)(tb->addTask(tr, task)); this->~ActorAddTaskActorState(); static_cast<ActorAddTaskActor*>(this)->destroy(); return 0; }
															#line 10334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<ActorAddTaskActor*>(this)->SAV< Key >::value()) Key(tb->addTask(tr, task));
		this->~ActorAddTaskActorState();
		static_cast<ActorAddTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && validationValue,int loopDepth) 
	{
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 10346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 10352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		TaskBucket::setValidationCondition(task, validationKey, validationValue.get());
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<ActorAddTaskActor*>(this)->SAV<Key>::futures) { (void)(tb->addTask(tr, task)); this->~ActorAddTaskActorState(); static_cast<ActorAddTaskActor*>(this)->destroy(); return 0; }
															#line 10358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskBucket* tb;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	KeyRef validationKey;
															#line 10437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via actorAddTask()
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class ActorAddTaskActor final : public Actor<Key>, public ActorCallback< ActorAddTaskActor, 0, Optional<Value> >, public FastAllocated<ActorAddTaskActor>, public ActorAddTaskActorState<ActorAddTaskActor> {
															#line 10442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<ActorAddTaskActor>::operator new;
	using FastAllocated<ActorAddTaskActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Key>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorAddTaskActor, 0, Optional<Value> >;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	ActorAddTaskActor(TaskBucket* const& tb,Reference<ReadYourWritesTransaction> const& tr,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 10453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Key> actorAddTask( TaskBucket* const& tb, Reference<ReadYourWritesTransaction> const& tr, Reference<Task> const& task, KeyRef const& validationKey ) {
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Key>(new ActorAddTaskActor(tb, tr, task, validationKey));
															#line 10481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

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

	Tuple t = Tuple::makeTuple(task->timeoutVersion, task->key);

	tr->atomicOp(prefix.pack("task_count"_sr), "\xff\xff\xff\xff\xff\xff\xff\xff"_sr, MutationRef::AddValue);
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
	return tr->watch(prefix.pack("task_count"_sr));
}

Future<Void> TaskBucket::debugPrintRange(Reference<ReadYourWritesTransaction> tr, Subspace subspace, Key msg) {
	return TaskBucketImpl::debugPrintRange(tr, subspace, msg);
}

class FutureBucketImpl {
public:
																#line 10570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isEmpty()
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsEmptyActor1>
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActor1State {
															#line 10576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActor1State(Reference<ReadYourWritesTransaction> const& tr,Reference<FutureBucket> const& futureBucket) 
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   futureBucket(futureBucket)
															#line 10585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			futureBucket->setOptions(tr);
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Key> __when_expr_0 = tr->getKey(lastLessOrEqual(futureBucket->prefix.pack(maxUIDKey)));
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<IsEmptyActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsEmptyActor1*>(this)->actor_wait_state = 1;
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsEmptyActor1, 0, Key >*>(static_cast<IsEmptyActor1*>(this)));
															#line 10609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor1*>(this)->SAV<bool>::futures) { (void)(!futureBucket->prefix.contains(lastKey)); this->~IsEmptyActor1State(); static_cast<IsEmptyActor1*>(this)->destroy(); return 0; }
															#line 10632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsEmptyActor1*>(this)->SAV< bool >::value()) bool(!futureBucket->prefix.contains(lastKey));
		this->~IsEmptyActor1State();
		static_cast<IsEmptyActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Key && lastKey,int loopDepth) 
	{
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsEmptyActor1*>(this)->SAV<bool>::futures) { (void)(!futureBucket->prefix.contains(lastKey)); this->~IsEmptyActor1State(); static_cast<IsEmptyActor1*>(this)->destroy(); return 0; }
															#line 10644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<FutureBucket> futureBucket;
															#line 10719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isEmpty()
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsEmptyActor1 final : public Actor<bool>, public ActorCallback< IsEmptyActor1, 0, Key >, public FastAllocated<IsEmptyActor1>, public IsEmptyActor1State<IsEmptyActor1> {
															#line 10724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsEmptyActor1>::operator new;
	using FastAllocated<IsEmptyActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsEmptyActor1, 0, Key >;
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsEmptyActor1(Reference<ReadYourWritesTransaction> const& tr,Reference<FutureBucket> const& futureBucket) 
															#line 10735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isEmpty( Reference<ReadYourWritesTransaction> const& tr, Reference<FutureBucket> const& futureBucket ) {
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsEmptyActor1(tr, futureBucket));
															#line 10762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
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
																#line 10799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via join()
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class JoinActor>
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinActorState {
															#line 10805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   vectorFuture(vectorFuture)
															#line 10818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = isSet(tr, taskFuture);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<JoinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<JoinActor*>(this)->actor_wait_state = 1;
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< JoinActor, 0, bool >*>(static_cast<JoinActor*>(this)));
															#line 10842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 10865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 10869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~JoinActorState();
			static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->blocks.pack(StringRef()));
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = _join(tr, taskBucket, taskFuture, vectorFuture);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<JoinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<JoinActor*>(this)->actor_wait_state = 2;
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< JoinActor, 1, Void >*>(static_cast<JoinActor*>(this)));
															#line 10886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool && is_set,int loopDepth) 
	{
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 10895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 10899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~JoinActorState();
			static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->blocks.pack(StringRef()));
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = _join(tr, taskBucket, taskFuture, vectorFuture);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<JoinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<JoinActor*>(this)->actor_wait_state = 2;
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< JoinActor, 1, Void >*>(static_cast<JoinActor*>(this)));
															#line 10916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 10988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<JoinActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~JoinActorState();
		static_cast<JoinActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~JoinActorState(); static_cast<JoinActor*>(this)->destroy(); return 0; }
															#line 11000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Reference<TaskFuture>> vectorFuture;
															#line 11079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via join()
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinActor final : public Actor<Void>, public ActorCallback< JoinActor, 0, bool >, public ActorCallback< JoinActor, 1, Void >, public FastAllocated<JoinActor>, public JoinActorState<JoinActor> {
															#line 11084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<JoinActor>::operator new;
	using FastAllocated<JoinActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< JoinActor, 0, bool >;
friend struct ActorCallback< JoinActor, 1, Void >;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 11096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> join( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, std::vector<Reference<TaskFuture>> const& vectorFuture ) {
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new JoinActor(tr, taskBucket, taskFuture, vectorFuture));
															#line 11124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via _join()
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class _joinActor>
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _joinActorState {
															#line 11135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_joinActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   vectorFuture(vectorFuture)
															#line 11148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			std::vector<Future<Void>> onSetFutures;
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for(int i = 0;i < vectorFuture.size();++i) {
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key key = StringRef(deterministicRandom()->randomUniqueID().toString());
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				taskFuture->addBlock(tr, key);
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				auto task = makeReference<Task>();
															#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[Task::reservedTaskParamKeyType] = "UnblockFuture"_sr;
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[Task::reservedTaskParamKeyFuture] = taskFuture->key;
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[Task::reservedTaskParamKeyBlockID] = key;
															#line 1104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				onSetFutures.push_back(vectorFuture[i]->onSet(tr, taskBucket, task));
															#line 11179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(onSetFutures);
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<_joinActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_joinActor*>(this)->actor_wait_state = 1;
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _joinActor, 0, Void >*>(static_cast<_joinActor*>(this)));
															#line 11190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<_joinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_joinActorState(); static_cast<_joinActor*>(this)->destroy(); return 0; }
															#line 11213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<_joinActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_joinActorState();
		static_cast<_joinActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<_joinActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_joinActorState(); static_cast<_joinActor*>(this)->destroy(); return 0; }
															#line 11225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	std::vector<Reference<TaskFuture>> vectorFuture;
															#line 11304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via _join()
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class _joinActor final : public Actor<Void>, public ActorCallback< _joinActor, 0, Void >, public FastAllocated<_joinActor>, public _joinActorState<_joinActor> {
															#line 11309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<_joinActor>::operator new;
	using FastAllocated<_joinActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _joinActor, 0, Void >;
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	_joinActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,std::vector<Reference<TaskFuture>> const& vectorFuture) 
															#line 11320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> _join( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, std::vector<Reference<TaskFuture>> const& vectorFuture ) {
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new _joinActor(tr, taskBucket, taskFuture, vectorFuture));
															#line 11347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via isSet()
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class IsSetActor>
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsSetActorState {
															#line 11358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsSetActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskFuture> const& taskFuture) 
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 11367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(taskFuture->blocks.range(), 1);
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<IsSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsSetActor*>(this)->actor_wait_state = 1;
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsSetActor, 0, RangeResult >*>(static_cast<IsSetActor*>(this)));
															#line 11391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 11414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsSetActorState();
			static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsSetActorState();
		static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() > 0)
															#line 11438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			new (&static_cast<IsSetActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsSetActorState();
			static_cast<IsSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<IsSetActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsSetActorState(); static_cast<IsSetActor*>(this)->destroy(); return 0; }
															#line 11450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 11525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via isSet()
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class IsSetActor final : public Actor<bool>, public ActorCallback< IsSetActor, 0, RangeResult >, public FastAllocated<IsSetActor>, public IsSetActorState<IsSetActor> {
															#line 11530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<IsSetActor>::operator new;
	using FastAllocated<IsSetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsSetActor, 0, RangeResult >;
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	IsSetActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskFuture> const& taskFuture) 
															#line 11541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<bool> isSet( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskFuture> const& taskFuture ) {
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<bool>(new IsSetActor(tr, taskFuture));
															#line 11568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via onSet()
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class OnSetActor>
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetActorState {
															#line 11579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 11592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<bool> __when_expr_0 = isSet(tr, taskFuture);
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSetActor*>(this)->actor_wait_state = 1;
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSetActor, 0, bool >*>(static_cast<OnSetActor*>(this)));
															#line 11616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 11639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "is_set == true");
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = performAction(tr, taskBucket, taskFuture, task);
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnSetActor*>(this)->actor_wait_state = 2;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetActor, 1, Void >*>(static_cast<OnSetActor*>(this)));
															#line 11652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "is_set == false");
															#line 1135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace callbackSpace = taskFuture->callbacks.get(StringRef(deterministicRandom()->randomUniqueID().toString()));
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& v : task->params ) {
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(callbackSpace.pack(v.key), v.value);
															#line 11665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(bool && is_set,int loopDepth) 
	{
															#line 1130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (is_set)
															#line 11676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "is_set == true");
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_1 = performAction(tr, taskBucket, taskFuture, task);
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnSetActor*>(this)->actor_wait_state = 2;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetActor, 1, Void >*>(static_cast<OnSetActor*>(this)));
															#line 11689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			CODE_PROBE(true, "is_set == false");
															#line 1135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Subspace callbackSpace = taskFuture->callbacks.get(StringRef(deterministicRandom()->randomUniqueID().toString()));
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& v : task->params ) {
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				tr->set(callbackSpace.pack(v.key), v.value);
															#line 11702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetActorState(); static_cast<OnSetActor*>(this)->destroy(); return 0; }
															#line 11776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 11867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via onSet()
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetActor final : public Actor<Void>, public ActorCallback< OnSetActor, 0, bool >, public ActorCallback< OnSetActor, 1, Void >, public FastAllocated<OnSetActor>, public OnSetActorState<OnSetActor> {
															#line 11872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<OnSetActor>::operator new;
	using FastAllocated<OnSetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSetActor, 0, bool >;
friend struct ActorCallback< OnSetActor, 1, Void >;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 11884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> onSet( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task ) {
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new OnSetActor(tr, taskBucket, taskFuture, task));
															#line 11912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 11917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via set()
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class SetActor>
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class SetActorState {
															#line 11923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	SetActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 11934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			tr->clear(taskFuture->blocks.range());
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = performAllActions(tr, taskBucket, taskFuture);
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<SetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetActor*>(this)->actor_wait_state = 1;
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetActor, 0, Void >*>(static_cast<SetActor*>(this)));
															#line 11960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<SetActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetActorState(); static_cast<SetActor*>(this)->destroy(); return 0; }
															#line 11983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<SetActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetActorState();
		static_cast<SetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<SetActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetActorState(); static_cast<SetActor*>(this)->destroy(); return 0; }
															#line 11995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 12072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via set()
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class SetActor final : public Actor<Void>, public ActorCallback< SetActor, 0, Void >, public FastAllocated<SetActor>, public SetActorState<SetActor> {
															#line 12077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<SetActor>::operator new;
	using FastAllocated<SetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetActor, 0, Void >;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	SetActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 12088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> set( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture ) {
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new SetActor(tr, taskBucket, taskFuture));
															#line 12115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via performAction()
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class PerformActionActor>
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformActionActorState {
															#line 12126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformActionActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 12139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (task && TaskFuncBase::isValidTask(task))
															#line 12156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			{
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Reference<TaskFuncBase> taskFunc = TaskFuncBase::create(task->params[Task::reservedTaskParamKeyType]);
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskFunc.getPtr())
															#line 12162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					StrictFuture<Void> __when_expr_0 = taskFunc->finish(tr, taskBucket, taskFuture->futureBucket, task);
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					if (static_cast<PerformActionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<PerformActionActor*>(this)->actor_wait_state = 1;
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PerformActionActor, 0, Void >*>(static_cast<PerformActionActor*>(this)));
															#line 12173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<PerformActionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PerformActionActorState(); static_cast<PerformActionActor*>(this)->destroy(); return 0; }
															#line 12206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 12303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via performAction()
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformActionActor final : public Actor<Void>, public ActorCallback< PerformActionActor, 0, Void >, public FastAllocated<PerformActionActor>, public PerformActionActorState<PerformActionActor> {
															#line 12308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<PerformActionActor>::operator new;
	using FastAllocated<PerformActionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PerformActionActor, 0, Void >;
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformActionActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 12319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> performAction( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task ) {
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new PerformActionActor(tr, taskBucket, taskFuture, task));
															#line 12346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via performAllActions()
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class PerformAllActionsActor>
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformAllActionsActorState {
															#line 12357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformAllActionsActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 12368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(taskFuture->callbacks.range(), CLIENT_KNOBS->TOO_MANY);
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 1;
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PerformAllActionsActor, 0, RangeResult >*>(static_cast<PerformAllActionsActor*>(this)));
															#line 12392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->callbacks.range());
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		std::vector<Future<Void>> actions;
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() != 0)
															#line 12419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task = Reference<Task>(new Task());
															#line 1185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key lastTaskID;
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& s : values ) {
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Tuple t = taskFuture->callbacks.unpack(s.key);
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key taskID = t.getString(0);
															#line 1189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key key = t.getString(1);
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskID.size() != 0 && taskID != lastTaskID)
															#line 12435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					task = makeReference<Task>();
															#line 12441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[key] = s.value;
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastTaskID = taskID;
															#line 12447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 12451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(actions);
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 2;
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PerformAllActionsActor, 1, Void >*>(static_cast<PerformAllActionsActor*>(this)));
															#line 12462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && values,int loopDepth) 
	{
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		tr->clear(taskFuture->callbacks.range());
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		std::vector<Future<Void>> actions;
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (values.size() != 0)
															#line 12475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task = Reference<Task>(new Task());
															#line 1185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			Key lastTaskID;
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			for( auto& s : values ) {
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Tuple t = taskFuture->callbacks.unpack(s.key);
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key taskID = t.getString(0);
															#line 1189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				Key key = t.getString(1);
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				if (taskID.size() != 0 && taskID != lastTaskID)
															#line 12491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				{
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
					task = makeReference<Task>();
															#line 12497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
				}
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				task->params[key] = s.value;
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
				lastTaskID = taskID;
															#line 12503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			}
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			actions.push_back(performAction(tr, taskBucket, taskFuture, task));
															#line 12507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(actions);
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<PerformAllActionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<PerformAllActionsActor*>(this)->actor_wait_state = 2;
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PerformAllActionsActor, 1, Void >*>(static_cast<PerformAllActionsActor*>(this)));
															#line 12518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<PerformAllActionsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PerformAllActionsActorState(); static_cast<PerformAllActionsActor*>(this)->destroy(); return 0; }
															#line 12590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<PerformAllActionsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PerformAllActionsActorState();
		static_cast<PerformAllActionsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<PerformAllActionsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PerformAllActionsActorState(); static_cast<PerformAllActionsActor*>(this)->destroy(); return 0; }
															#line 12602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 12681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via performAllActions()
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class PerformAllActionsActor final : public Actor<Void>, public ActorCallback< PerformAllActionsActor, 0, RangeResult >, public ActorCallback< PerformAllActionsActor, 1, Void >, public FastAllocated<PerformAllActionsActor>, public PerformAllActionsActorState<PerformAllActionsActor> {
															#line 12686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<PerformAllActionsActor>::operator new;
	using FastAllocated<PerformAllActionsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PerformAllActionsActor, 0, RangeResult >;
friend struct ActorCallback< PerformAllActionsActor, 1, Void >;
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	PerformAllActionsActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 12698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> performAllActions( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture ) {
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new PerformAllActionsActor(tr, taskBucket, taskFuture));
															#line 12726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via onSetAddTask()
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class OnSetAddTaskActor>
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActorState {
															#line 12737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task)
															#line 12750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[Task::reservedTaskParamKeyAddTask] = task->params[Task::reservedTaskParamKeyType];
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			task->params[Task::reservedTaskParamKeyType] = "AddTask"_sr;
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = onSet(tr, taskBucket, taskFuture, task);
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetAddTaskActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSetAddTaskActor*>(this)->actor_wait_state = 1;
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor, 0, Void >*>(static_cast<OnSetAddTaskActor*>(this)));
															#line 12778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActorState(); static_cast<OnSetAddTaskActor*>(this)->destroy(); return 0; }
															#line 12801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetAddTaskActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetAddTaskActorState();
		static_cast<OnSetAddTaskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActorState(); static_cast<OnSetAddTaskActor*>(this)->destroy(); return 0; }
															#line 12813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 12892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via onSetAddTask()
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActor final : public Actor<Void>, public ActorCallback< OnSetAddTaskActor, 0, Void >, public FastAllocated<OnSetAddTaskActor>, public OnSetAddTaskActorState<OnSetAddTaskActor> {
															#line 12897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<OnSetAddTaskActor>::operator new;
	using FastAllocated<OnSetAddTaskActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSetAddTaskActor, 0, Void >;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task) 
															#line 12908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> onSetAddTask( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task ) {
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new OnSetAddTaskActor(tr, taskBucket, taskFuture, task));
															#line 12935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

																#line 12940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via onSetAddTask()
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class OnSetAddTaskActor1>
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActor1State {
															#line 12946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActor1State(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture),
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   task(task),
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   validationKey(validationKey)
															#line 12961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(validationKey);
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 1;
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >*>(static_cast<OnSetAddTaskActor1*>(this)));
															#line 12985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 13008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketOnSetAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 13014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidKey] = validationKey;
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidValue] = validationValue.get();
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = onSetAddTask(tr, taskBucket, taskFuture, task);
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 2;
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor1, 1, Void >*>(static_cast<OnSetAddTaskActor1*>(this)));
															#line 13029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && validationValue,int loopDepth) 
	{
															#line 1230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!validationValue.present())
															#line 13038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		{
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			TraceEvent(SevError, "TaskBucketOnSetAddTaskInvalidKey") .detail("Task", task->params[Task::reservedTaskParamKeyType]) .detail("ValidationKey", validationKey);
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			return a_body1Catch1(invalid_option_value(), loopDepth);
															#line 13044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		}
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidKey] = validationKey;
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		task->params[Task::reservedTaskParamValidValue] = validationValue.get();
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		StrictFuture<Void> __when_expr_1 = onSetAddTask(tr, taskBucket, taskFuture, task);
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSetAddTaskActor1*>(this)->actor_wait_state = 2;
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSetAddTaskActor1, 1, Void >*>(static_cast<OnSetAddTaskActor1*>(this)));
															#line 13059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActor1State(); static_cast<OnSetAddTaskActor1*>(this)->destroy(); return 0; }
															#line 13131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<OnSetAddTaskActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSetAddTaskActor1State();
		static_cast<OnSetAddTaskActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<OnSetAddTaskActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSetAddTaskActor1State(); static_cast<OnSetAddTaskActor1*>(this)->destroy(); return 0; }
															#line 13143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<Task> task;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	KeyRef validationKey;
															#line 13224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via onSetAddTask()
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class OnSetAddTaskActor1 final : public Actor<Void>, public ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >, public ActorCallback< OnSetAddTaskActor1, 1, Void >, public FastAllocated<OnSetAddTaskActor1>, public OnSetAddTaskActor1State<OnSetAddTaskActor1> {
															#line 13229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<OnSetAddTaskActor1>::operator new;
	using FastAllocated<OnSetAddTaskActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSetAddTaskActor1, 0, Optional<Value> >;
friend struct ActorCallback< OnSetAddTaskActor1, 1, Void >;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	OnSetAddTaskActor1(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture,Reference<Task> const& task,KeyRef const& validationKey) 
															#line 13241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Void> onSetAddTask( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture, Reference<Task> const& task, KeyRef const& validationKey ) {
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Void>(new OnSetAddTaskActor1(tr, taskBucket, taskFuture, task, validationKey));
															#line 13269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

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

																#line 13288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
// This generated class is to be used only via joinedFuture()
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class JoinedFutureActor>
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinedFutureActorState {
															#line 13294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinedFutureActorState(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : tr(tr),
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskBucket(taskBucket),
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   taskFuture(taskFuture)
															#line 13305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			taskFuture->futureBucket->setOptions(tr);
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			std::vector<Reference<TaskFuture>> vectorFuture;
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			future = taskFuture->futureBucket->future(tr);
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			vectorFuture.push_back(future);
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Void> __when_expr_0 = join(tr, taskBucket, taskFuture, vectorFuture);
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<JoinedFutureActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<JoinedFutureActor*>(this)->actor_wait_state = 1;
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< JoinedFutureActor, 0, Void >*>(static_cast<JoinedFutureActor*>(this)));
															#line 13335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinedFutureActor*>(this)->SAV<Reference<TaskFuture>>::futures) { (void)(future); this->~JoinedFutureActorState(); static_cast<JoinedFutureActor*>(this)->destroy(); return 0; }
															#line 13358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<JoinedFutureActor*>(this)->SAV< Reference<TaskFuture> >::value()) Reference<TaskFuture>(std::move(future)); // state_var_RVO
		this->~JoinedFutureActorState();
		static_cast<JoinedFutureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<JoinedFutureActor*>(this)->SAV<Reference<TaskFuture>>::futures) { (void)(future); this->~JoinedFutureActorState(); static_cast<JoinedFutureActor*>(this)->destroy(); return 0; }
															#line 13370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskBucket> taskBucket;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> taskFuture;
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Reference<TaskFuture> future;
															#line 13449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via joinedFuture()
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class JoinedFutureActor final : public Actor<Reference<TaskFuture>>, public ActorCallback< JoinedFutureActor, 0, Void >, public FastAllocated<JoinedFutureActor>, public JoinedFutureActorState<JoinedFutureActor> {
															#line 13454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<JoinedFutureActor>::operator new;
	using FastAllocated<JoinedFutureActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<TaskFuture>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< JoinedFutureActor, 0, Void >;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	JoinedFutureActor(Reference<ReadYourWritesTransaction> const& tr,Reference<TaskBucket> const& taskBucket,Reference<TaskFuture> const& taskFuture) 
															#line 13465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] static Future<Reference<TaskFuture>> joinedFuture( Reference<ReadYourWritesTransaction> const& tr, Reference<TaskBucket> const& taskBucket, Reference<TaskFuture> const& taskFuture ) {
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Reference<TaskFuture>>(new JoinedFutureActor(tr, taskBucket, taskFuture));
															#line 13492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
};

TaskFuture::TaskFuture() {}

TaskFuture::TaskFuture(const Reference<FutureBucket> bucket, Key k) : futureBucket(bucket), key(k) {
	if (k.size() == 0) {
		key = deterministicRandom()->randomUniqueID().toString();
	}

	prefix = futureBucket->prefix.get(key);
	blocks = prefix.get("bl"_sr);
	callbacks = prefix.get("cb"_sr);
}

TaskFuture::~TaskFuture() {}

void TaskFuture::addBlock(Reference<ReadYourWritesTransaction> tr, StringRef block_id) {
	tr->set(blocks.pack(block_id), ""_sr);
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

															#line 13567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
namespace {
// This generated class is to be used only via getCompletionKey()
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
template <class GetCompletionKeyActor>
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetCompletionKeyActorState {
															#line 13574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetCompletionKeyActorState(TaskCompletionKey* const& self,Future<Reference<TaskFuture>> const& f) 
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		 : self(self),
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		   f(f)
															#line 13583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			StrictFuture<Reference<TaskFuture>> __when_expr_0 = f;
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			if (static_cast<GetCompletionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetCompletionKeyActor*>(this)->actor_wait_state = 1;
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >*>(static_cast<GetCompletionKeyActor*>(this)));
															#line 13605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->joinFuture.clear();
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->key = taskFuture->key;
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetCompletionKeyActor*>(this)->SAV<Key>::futures) { (void)(self->key.get()); this->~GetCompletionKeyActorState(); static_cast<GetCompletionKeyActor*>(this)->destroy(); return 0; }
															#line 13632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
		new (&static_cast<GetCompletionKeyActor*>(this)->SAV< Key >::value()) Key(self->key.get());
		this->~GetCompletionKeyActorState();
		static_cast<GetCompletionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<TaskFuture> && taskFuture,int loopDepth) 
	{
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->joinFuture.clear();
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		self->key = taskFuture->key;
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
		if (!static_cast<GetCompletionKeyActor*>(this)->SAV<Key>::futures) { (void)(self->key.get()); this->~GetCompletionKeyActorState(); static_cast<GetCompletionKeyActor*>(this)->destroy(); return 0; }
															#line 13648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	TaskCompletionKey* self;
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	Future<Reference<TaskFuture>> f;
															#line 13723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
};
// This generated class is to be used only via getCompletionKey()
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
class GetCompletionKeyActor final : public Actor<Key>, public ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >, public FastAllocated<GetCompletionKeyActor>, public GetCompletionKeyActorState<GetCompletionKeyActor> {
															#line 13728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
public:
	using FastAllocated<GetCompletionKeyActor>::operator new;
	using FastAllocated<GetCompletionKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Key>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetCompletionKeyActor, 0, Reference<TaskFuture> >;
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	GetCompletionKeyActor(TaskCompletionKey* const& self,Future<Reference<TaskFuture>> const& f) 
															#line 13739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
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
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
[[nodiscard]] Future<Key> getCompletionKey( TaskCompletionKey* const& self, Future<Reference<TaskFuture>> const& f ) {
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"
	return Future<Key>(new GetCompletionKeyActor(self, f));
															#line 13767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.g.cpp"
}

#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TaskBucket.actor.cpp"

Future<Key> TaskCompletionKey::get(Reference<ReadYourWritesTransaction> tr, Reference<TaskBucket> taskBucket) {
	ASSERT(key.present() == (joinFuture.getPtr() == nullptr));
	return key.present() ? key.get() : getCompletionKey(this, joinFuture->joinedFuture(tr, taskBucket));
}
