#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
/*
 * MonitorLeader.actor.cpp
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

#include "fdbclient/ClusterConnectionMemoryRecord.h"
#include "fdbclient/MonitorLeader.h"
#include "fdbclient/CoordinationInterface.h"
#include "fdbclient/NativeAPI.actor.h"
#include "flow/ActorCollection.h"
#include "flow/UnitTest.h"
#include "fdbrpc/genericactors.actor.h"
#include "flow/Platform.h"
#include "flow/actorcompiler.h" // has to be last include

namespace {

std::string trim(std::string const& connectionString) {
	// Strip out whitespace
	// Strip out characters between a # and a newline
	std::string trimmed;
	auto end = connectionString.end();
	for (auto c = connectionString.begin(); c != end; ++c) {
		if (*c == '#') {
			++c;
			while (c != end && *c != '\n' && *c != '\r')
				++c;
			if (c == end)
				break;
		} else if (*c != ' ' && *c != '\n' && *c != '\r' && *c != '\t')
			trimmed += *c;
	}
	return trimmed;
}

} // namespace

FDB_DEFINE_BOOLEAN_PARAM(ConnectionStringNeedsPersisted);

// Returns the connection string currently held in this object. This may not match the stored record if it hasn't
// been persisted or if the persistent storage for the record has been modified externally.
ClusterConnectionString& IClusterConnectionRecord::getConnectionString() {
	return cs;
}

Future<bool> IClusterConnectionRecord::upToDate() {
	ClusterConnectionString temp;
	return upToDate(temp);
}

void IClusterConnectionRecord::notifyConnected() {
	if (connectionStringNeedsPersisted) {
		this->persist();
	}
}

bool IClusterConnectionRecord::needsToBePersisted() const {
	return connectionStringNeedsPersisted;
}

void IClusterConnectionRecord::setPersisted() {
	connectionStringNeedsPersisted = false;
}

std::string ClusterConnectionString::getErrorString(std::string const& source, Error const& e) {
	if (e.code() == error_code_connection_string_invalid) {
		return format("Invalid connection string `%s: %d %s", source.c_str(), e.code(), e.what());
	} else {
		return format("Unexpected error parsing connection string `%s: %d %s", source.c_str(), e.code(), e.what());
	}
}

ClusterConnectionString::ClusterConnectionString(const std::string& connectionString) {
	auto trimmed = trim(connectionString);
	// Split on '@' into key@addrs
	int pAt = trimmed.find_first_of('@');
	if (pAt == trimmed.npos) {
		throw connection_string_invalid();
	}
	std::string key = trimmed.substr(0, pAt);
	std::string addrs = trimmed.substr(pAt + 1);

	parseKey(key);
	std::set<Hostname> hostnameSet;
	std::set<NetworkAddress> addressSet;
	std::string curAddr;
	for (int p = 0; p <= addrs.size();) {
		int pComma = addrs.find_first_of(',', p);
		if (pComma == addrs.npos)
			pComma = addrs.size();
		curAddr = addrs.substr(p, pComma - p);
		if (Hostname::isHostname(curAddr)) {
			Hostname h = Hostname::parse(curAddr);
			// Check that there are no duplicate hostnames
			if (hostnameSet.find(h) != hostnameSet.end()) {
				throw connection_string_invalid();
			}
			hostnames.push_back(Hostname::parse(curAddr));
			hostnameSet.insert(h);
		} else {
			NetworkAddress n = NetworkAddress::parse(curAddr);
			// Check that there are no duplicate addresses
			if (addressSet.find(n) != addressSet.end()) {
				throw connection_string_invalid();
			}
			coords.push_back(n);
			addressSet.insert(n);
		}
		p = pComma + 1;
	}
	ASSERT((coords.size() + hostnames.size()) > 0);
}

															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase129()
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase129Actor>
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase129ActorState {
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase129ActorState(UnitTestParameters const& params) 
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params),
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   input()
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase129", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase129ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase129", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@1.1.1.1:345";
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(input);
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(input == cs.toString());
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
			{
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@1.1.1.1:345,1.1.1.1:345";
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				try {
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					ClusterConnectionString cs(input);
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					loopDepth = a_body1cont5(loopDepth);
				}
				catch (Error& error) {
					loopDepth = a_body1Catch2(error, loopDepth);
				} catch (...) {
					loopDepth = a_body1Catch2(unknown_error(), loopDepth);
				}
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
		this->~FlowTestCase129ActorState();
		static_cast<FlowTestCase129Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@1.1.1.1:34534,5.1.5.3:23443";
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(input);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@1.1.1.1:34534,5.1.5.3:23443";
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@[::1]:1234,[::1]:1235";
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@[abcd:dcba::1]:1234,[abcd:dcba::abcd:1]:1234";
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase129Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase129ActorState(); static_cast<FlowTestCase129Actor*>(this)->destroy(); return 0; }
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase129Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase129ActorState();
		static_cast<FlowTestCase129Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(e.code() == error_code_connection_string_invalid);
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::string input;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase129()
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase129Actor final : public Actor<Void>, public FastAllocated<FlowTestCase129Actor>, public FlowTestCase129ActorState<FlowTestCase129Actor> {
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase129Actor>::operator new;
	using FastAllocated<FlowTestCase129Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase129Actor(UnitTestParameters const& params) 
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase129ActorState<FlowTestCase129Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase129", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase129");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase129", reinterpret_cast<unsigned long>(this), -1);

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
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase129( UnitTestParameters const& params ) {
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase129Actor(params));
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase129, "/fdbclient/MonitorLeader/parseConnectionString/addresses")

#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase189()
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase189Actor>
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase189ActorState {
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase189ActorState(UnitTestParameters const& params) 
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params),
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   input()
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase189", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase189ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase189", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@localhost:1234";
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(input);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.hostnames.size() == 1);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(input == cs.toString());
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
			{
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@localhost:1234,localhost:1234";
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				try {
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					ClusterConnectionString cs(input);
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					loopDepth = a_body1cont5(loopDepth);
				}
				catch (Error& error) {
					loopDepth = a_body1Catch2(error, loopDepth);
				} catch (...) {
					loopDepth = a_body1Catch2(unknown_error(), loopDepth);
				}
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
		this->~FlowTestCase189ActorState();
		static_cast<FlowTestCase189Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@localhost:34534,host-name:23443";
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(input);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(cs.hostnames.size() == 2);
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@localhost:34534,host-name:23443";
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(cs.hostnames.size() == 2);
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@localhost:34534,host-name_part1.host-name_part2:1234:tls";
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(cs.hostnames.size() == 2);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase189Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase189ActorState(); static_cast<FlowTestCase189Actor*>(this)->destroy(); return 0; }
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase189Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase189ActorState();
		static_cast<FlowTestCase189Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(e.code() == error_code_connection_string_invalid);
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::string input;
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase189()
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase189Actor final : public Actor<Void>, public FastAllocated<FlowTestCase189Actor>, public FlowTestCase189ActorState<FlowTestCase189Actor> {
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase189Actor>::operator new;
	using FastAllocated<FlowTestCase189Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase189Actor(UnitTestParameters const& params) 
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase189ActorState<FlowTestCase189Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase189", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase189");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase189", reinterpret_cast<unsigned long>(this), -1);

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
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase189( UnitTestParameters const& params ) {
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase189Actor(params));
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase189, "/fdbclient/MonitorLeader/parseConnectionString/hostnames")

#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase242()
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase242Actor>
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase242ActorState {
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase242ActorState(UnitTestParameters const& params) 
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase242", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase242ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase242", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = "TestCluster:0@localhost:1234,host-name:5678";
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string hn1 = "localhost", port1 = "1234", hn2 = "host-name", port2 = "5678";
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::vector<Hostname> hostnames;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn1 + ":" + port1));
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn2 + ":" + port2));
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(hostnames, LiteralStringRef("TestCluster:0"));
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.hostnames.size() == 2);
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.coords.size() == 0);
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.toString() == connectionString);
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
			{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.clear();
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn1 + ":" + port1));
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn1 + ":" + port1));
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				try {
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					ClusterConnectionString cs(hostnames, LiteralStringRef("TestCluster:0"));
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					loopDepth = a_body1cont5(loopDepth);
				}
				catch (Error& error) {
					loopDepth = a_body1Catch2(error, loopDepth);
				} catch (...) {
					loopDepth = a_body1Catch2(unknown_error(), loopDepth);
				}
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
		this->~FlowTestCase242ActorState();
		static_cast<FlowTestCase242Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase242Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase242ActorState(); static_cast<FlowTestCase242Actor*>(this)->destroy(); return 0; }
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase242Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase242ActorState();
		static_cast<FlowTestCase242Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(e.code() == error_code_connection_string_invalid);
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase242()
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase242Actor final : public Actor<Void>, public FastAllocated<FlowTestCase242Actor>, public FlowTestCase242ActorState<FlowTestCase242Actor> {
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase242Actor>::operator new;
	using FastAllocated<FlowTestCase242Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase242Actor(UnitTestParameters const& params) 
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase242ActorState<FlowTestCase242Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase242", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase242");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase242", reinterpret_cast<unsigned long>(this), -1);

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
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase242( UnitTestParameters const& params ) {
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase242Actor(params));
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase242, "/fdbclient/MonitorLeader/ConnectionString/hostname")

#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via tryResolveHostnamesImpl()
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class TryResolveHostnamesImplActor>
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class TryResolveHostnamesImplActorState {
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	TryResolveHostnamesImplActorState(ClusterConnectionString* const& self) 
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : self(self),
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allCoordinatorsSet()
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this));

	}
	~TryResolveHostnamesImplActorState() 
	{
		fdb_probe_actor_destroy("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( const auto& coord : self->coords ) {
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				allCoordinatorsSet.insert(coord);
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::vector<Future<Void>> fs;
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( auto& hostname : self->hostnames ) {
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				fs.push_back(map(hostname.resolve(), [&](Optional<NetworkAddress> const& addr) -> Void { if (addr.present()) { allCoordinatorsSet.insert(addr.get()); } return Void(); }));
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(fs);
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<TryResolveHostnamesImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryResolveHostnamesImplActor*>(this)->actor_wait_state = 1;
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryResolveHostnamesImplActor, 0, Void >*>(static_cast<TryResolveHostnamesImplActor*>(this)));
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~TryResolveHostnamesImplActorState();
		static_cast<TryResolveHostnamesImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::vector<NetworkAddress> allCoordinators(allCoordinatorsSet.begin(), allCoordinatorsSet.end());
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::sort(allCoordinators.begin(), allCoordinators.end());
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<TryResolveHostnamesImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(allCoordinators); this->~TryResolveHostnamesImplActorState(); static_cast<TryResolveHostnamesImplActor*>(this)->destroy(); return 0; }
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<TryResolveHostnamesImplActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(allCoordinators);
		this->~TryResolveHostnamesImplActorState();
		static_cast<TryResolveHostnamesImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::vector<NetworkAddress> allCoordinators(allCoordinatorsSet.begin(), allCoordinatorsSet.end());
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::sort(allCoordinators.begin(), allCoordinators.end());
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<TryResolveHostnamesImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(allCoordinators); this->~TryResolveHostnamesImplActorState(); static_cast<TryResolveHostnamesImplActor*>(this)->destroy(); return 0; }
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<TryResolveHostnamesImplActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(allCoordinators);
		this->~TryResolveHostnamesImplActorState();
		static_cast<TryResolveHostnamesImplActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TryResolveHostnamesImplActor*>(this)->actor_wait_state > 0) static_cast<TryResolveHostnamesImplActor*>(this)->actor_wait_state = 0;
		static_cast<TryResolveHostnamesImplActor*>(this)->ActorCallback< TryResolveHostnamesImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TryResolveHostnamesImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryResolveHostnamesImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryResolveHostnamesImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClusterConnectionString* self;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::set<NetworkAddress> allCoordinatorsSet;
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via tryResolveHostnamesImpl()
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class TryResolveHostnamesImplActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< TryResolveHostnamesImplActor, 0, Void >, public FastAllocated<TryResolveHostnamesImplActor>, public TryResolveHostnamesImplActorState<TryResolveHostnamesImplActor> {
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<TryResolveHostnamesImplActor>::operator new;
	using FastAllocated<TryResolveHostnamesImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryResolveHostnamesImplActor, 0, Void >;
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	TryResolveHostnamesImplActor(ClusterConnectionString* const& self) 
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<std::vector<NetworkAddress>>(),
		   TryResolveHostnamesImplActorState<TryResolveHostnamesImplActor>(self)
	{
		fdb_probe_actor_enter("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryResolveHostnamesImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryResolveHostnamesImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryResolveHostnamesImplActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<std::vector<NetworkAddress>> tryResolveHostnamesImpl( ClusterConnectionString* const& self ) {
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new TryResolveHostnamesImplActor(self));
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

Future<std::vector<NetworkAddress>> ClusterConnectionString::tryResolveHostnames() {
	return tryResolveHostnamesImpl(this);
}

															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase295()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase295Actor>
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase295ActorState {
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase295ActorState(UnitTestParameters const& params) 
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase295", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase295ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase295", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = "TestCluster:0@host.name:1234,host-name:5678";
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string hn = "host-name", port = "5678";
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			address = NetworkAddress::parse("1.0.0.0:5678");
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			INetworkConnections::net()->addMockTCPEndpoint(hn, port, { address });
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(connectionString);
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = cs.tryResolveHostnames();
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<FlowTestCase295Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase295Actor*>(this)->actor_wait_state = 1;
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >*>(static_cast<FlowTestCase295Actor*>(this)));
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~FlowTestCase295ActorState();
		static_cast<FlowTestCase295Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<NetworkAddress> const& allCoordinators,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		ASSERT(allCoordinators.size() == 1 && std::find(allCoordinators.begin(), allCoordinators.end(), address) != allCoordinators.end());
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase295Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase295ActorState(); static_cast<FlowTestCase295Actor*>(this)->destroy(); return 0; }
															#line 1059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase295Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase295ActorState();
		static_cast<FlowTestCase295Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<NetworkAddress> && allCoordinators,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		ASSERT(allCoordinators.size() == 1 && std::find(allCoordinators.begin(), allCoordinators.end(), address) != allCoordinators.end());
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase295Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase295ActorState(); static_cast<FlowTestCase295Actor*>(this)->destroy(); return 0; }
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase295Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase295ActorState();
		static_cast<FlowTestCase295Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> const& allCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont1(allCoordinators, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> && allCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(allCoordinators), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase295Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase295Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase295Actor*>(this)->ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase295", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase295", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("flowTestCase295", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase295", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase295", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase295", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	NetworkAddress address;
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase295()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase295Actor final : public Actor<Void>, public ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >, public FastAllocated<FlowTestCase295Actor>, public FlowTestCase295ActorState<FlowTestCase295Actor> {
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase295Actor>::operator new;
	using FastAllocated<FlowTestCase295Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase295Actor(UnitTestParameters const& params) 
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase295ActorState<FlowTestCase295Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase295", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase295");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase295", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase295Actor, 0, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase295( UnitTestParameters const& params ) {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase295Actor(params));
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase295, "/fdbclient/MonitorLeader/PartialResolve")

#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 1198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase311()
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase311Actor>
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase311ActorState {
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase311ActorState(UnitTestParameters const& params) 
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase311", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase311ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase311", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				LeaderInfo in;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				LeaderInfo out;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				in.forward = deterministicRandom()->coinflip();
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				in.changeID = deterministicRandom()->randomUniqueID();
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					std::string rndString(deterministicRandom()->randomInt(10, 400), 'x');
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					for( auto& c : rndString ) {
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						c = deterministicRandom()->randomAlphaNumeric();
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					in.serializedInfo = rndString;
															#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ObjectWriter writer(IncludeVersion());
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				writer.serialize(in);
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				Standalone<StringRef> copy = writer.toStringRef();
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ArenaObjectReader reader(copy.arena(), copy, IncludeVersion());
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				reader.deserialize(out);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(in.forward == out.forward);
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(in.changeID == out.changeID);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(in.serializedInfo == out.serializedInfo);
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			LeaderInfo leaderInfo;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			leaderInfo.forward = deterministicRandom()->coinflip();
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			leaderInfo.changeID = deterministicRandom()->randomUniqueID();
															#line 1272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				std::string rndString(deterministicRandom()->randomInt(10, 400), 'x');
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				for( auto& c : rndString ) {
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					c = deterministicRandom()->randomAlphaNumeric();
															#line 1280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				leaderInfo.serializedInfo = rndString;
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ErrorOr<EnsureTable<Optional<LeaderInfo>>> objIn(leaderInfo);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ErrorOr<EnsureTable<Optional<LeaderInfo>>> objOut;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			Standalone<StringRef> copy;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ObjectWriter writer(IncludeVersion());
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			writer.serialize(objIn);
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			copy = writer.toStringRef();
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ArenaObjectReader reader(copy.arena(), copy, IncludeVersion());
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			reader.deserialize(objOut);
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(!objOut.isError());
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(objOut.get().asUnderlyingType().present());
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			LeaderInfo outLeader = objOut.get().asUnderlyingType().get();
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(outLeader.changeID == leaderInfo.changeID);
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(outLeader.forward == leaderInfo.forward);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(outLeader.serializedInfo == leaderInfo.serializedInfo);
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (!static_cast<FlowTestCase311Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase311ActorState(); static_cast<FlowTestCase311Actor*>(this)->destroy(); return 0; }
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			new (&static_cast<FlowTestCase311Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase311ActorState();
			static_cast<FlowTestCase311Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase311ActorState();
		static_cast<FlowTestCase311Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 1340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase311()
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase311Actor final : public Actor<Void>, public FastAllocated<FlowTestCase311Actor>, public FlowTestCase311ActorState<FlowTestCase311Actor> {
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase311Actor>::operator new;
	using FastAllocated<FlowTestCase311Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase311Actor(UnitTestParameters const& params) 
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase311ActorState<FlowTestCase311Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase311", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase311");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase311", reinterpret_cast<unsigned long>(this), -1);

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
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase311( UnitTestParameters const& params ) {
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase311Actor(params));
															#line 1382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase311, "/flow/FlatBuffers/LeaderInfo")

#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 1388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase361()
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase361Actor>
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase361ActorState {
															#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase361ActorState(UnitTestParameters const& params) 
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 1402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase361", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase361ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase361", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = "0xxdeadbeef:100100100@1.1.1.1:34534,5.1.5.3:23443";
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for(int i = 0;i < 10000;i++) {
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				std::string output("");
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				auto c = connectionString.begin();
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				for(;c != connectionString.end();) {
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					if (deterministicRandom()->random01() < 0.1)
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					{
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += deterministicRandom()->randomChoice(LiteralStringRef(" \t\n\r"));
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					if (deterministicRandom()->random01() < 0.5)
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += *c;
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						++c;
															#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					if (deterministicRandom()->random01() < 0.1)
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					{
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += "#";
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						int charCount = deterministicRandom()->randomInt(0, 20);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						for(int i = 0;i < charCount;i++) {
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
							output += deterministicRandom()->randomChoice(LiteralStringRef("asdfzxcv123345:!@#$#$&()<\"\' \t"));
															#line 1455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
						}
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += deterministicRandom()->randomChoice(LiteralStringRef("\n\r"));
															#line 1459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
				}
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(output);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(connectionString == cs.toString());
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (!static_cast<FlowTestCase361Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase361ActorState(); static_cast<FlowTestCase361Actor*>(this)->destroy(); return 0; }
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			new (&static_cast<FlowTestCase361Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase361ActorState();
			static_cast<FlowTestCase361Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase361ActorState();
		static_cast<FlowTestCase361Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 1494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase361()
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase361Actor final : public Actor<Void>, public FastAllocated<FlowTestCase361Actor>, public FlowTestCase361ActorState<FlowTestCase361Actor> {
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase361Actor>::operator new;
	using FastAllocated<FlowTestCase361Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase361Actor(UnitTestParameters const& params) 
															#line 1509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase361ActorState<FlowTestCase361Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase361", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase361");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase361", reinterpret_cast<unsigned long>(this), -1);

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
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase361( UnitTestParameters const& params ) {
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase361Actor(params));
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase361, "/fdbclient/MonitorLeader/parseConnectionString/fuzz")

#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

ClusterConnectionString::ClusterConnectionString(const std::vector<NetworkAddress>& servers, Key key)
  : coords(servers) {
	std::set<NetworkAddress> s(servers.begin(), servers.end());
	if (s.size() != servers.size()) {
		throw connection_string_invalid();
	}
	std::string keyString = key.toString();
	parseKey(keyString);
}

ClusterConnectionString::ClusterConnectionString(const std::vector<Hostname>& hosts, Key key) : hostnames(hosts) {
	std::set<Hostname> h(hosts.begin(), hosts.end());
	if (h.size() != hosts.size()) {
		throw connection_string_invalid();
	}
	std::string keyString = key.toString();
	parseKey(keyString);
}

void ClusterConnectionString::parseKey(const std::string& key) {
	// Check the structure of the given key, and fill in this->key and this->keyDesc

	// The key must contain one (and only one) : character
	int colon = key.find_first_of(':');
	if (colon == key.npos) {
		throw connection_string_invalid();
	}
	std::string desc = key.substr(0, colon);
	std::string id = key.substr(colon + 1);

	// Check that description contains only allowed characters (a-z, A-Z, 0-9, _)
	for (auto c = desc.begin(); c != desc.end(); ++c) {
		if (!(isalnum(*c) || *c == '_')) {
			throw connection_string_invalid();
		}
	}

	// Check that ID contains only allowed characters (a-z, A-Z, 0-9)
	for (auto c = id.begin(); c != id.end(); ++c) {
		if (!isalnum(*c)) {
			throw connection_string_invalid();
		}
	}

	this->key = StringRef(key);
	this->keyDesc = StringRef(desc);
}

std::string ClusterConnectionString::toString() const {
	std::string s = key.toString();
	s += '@';
	for (auto const& coord : coords) {
		if (s.find('@') != s.length() - 1) {
			s += ',';
		}
		s += coord.toString();
	}
	for (auto const& host : hostnames) {
		if (s.find('@') != s.length() - 1) {
			s += ',';
		}
		s += host.toString();
	}
	return s;
}

ClientCoordinators::ClientCoordinators(Reference<IClusterConnectionRecord> ccr) : ccr(ccr) {
	ClusterConnectionString cs = ccr->getConnectionString();
	clusterKey = cs.clusterKey();
	for (auto h : cs.hostnames) {
		clientLeaderServers.push_back(ClientLeaderRegInterface(h));
	}
	for (auto s : cs.coords) {
		clientLeaderServers.push_back(ClientLeaderRegInterface(s));
	}
}

ClientCoordinators::ClientCoordinators(Key clusterKey, std::vector<NetworkAddress> coordinators)
  : clusterKey(clusterKey) {
	for (const auto& coord : coordinators) {
		clientLeaderServers.push_back(ClientLeaderRegInterface(coord));
	}
	ccr = makeReference<ClusterConnectionMemoryRecord>(ClusterConnectionString(coordinators, clusterKey));
}

ClientLeaderRegInterface::ClientLeaderRegInterface(NetworkAddress remote)
  : getLeader(Endpoint::wellKnown({ remote }, WLTOKEN_CLIENTLEADERREG_GETLEADER)),
    openDatabase(Endpoint::wellKnown({ remote }, WLTOKEN_CLIENTLEADERREG_OPENDATABASE)),
    checkDescriptorMutable(Endpoint::wellKnown({ remote }, WLTOKEN_CLIENTLEADERREG_DESCRIPTOR_MUTABLE)) {}

ClientLeaderRegInterface::ClientLeaderRegInterface(INetwork* local) {
	getLeader.makeWellKnownEndpoint(WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::Coordination);
	openDatabase.makeWellKnownEndpoint(WLTOKEN_CLIENTLEADERREG_OPENDATABASE, TaskPriority::Coordination);
	checkDescriptorMutable.makeWellKnownEndpoint(WLTOKEN_CLIENTLEADERREG_DESCRIPTOR_MUTABLE,
	                                             TaskPriority::Coordination);
}

// Nominee is the worker among all workers that are considered as leader by one coordinator
// This function contacts a coordinator coord to ask who is its nominee.
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorNominee()
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorNomineeActor>
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorNomineeActorState {
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorNomineeActorState(Key const& key,ClientLeaderRegInterface const& coord,AsyncTrigger* const& nomineeChange,Optional<LeaderInfo>* const& info) 
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : key(key),
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coord(coord),
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nomineeChange(nomineeChange),
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(info)
															#line 1661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("monitorNominee", reinterpret_cast<unsigned long>(this));

	}
	~MonitorNomineeActorState() 
	{
		fdb_probe_actor_destroy("monitorNominee", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~MonitorNomineeActorState();
		static_cast<MonitorNomineeActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		li = Optional<LeaderInfo>();
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (coord.hostname.present())
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_0 = store(li, retryGetReplyFromHostname(GetLeaderRequest(key, info->present() ? info->get().changeID : UID()), coord.hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 1;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 0, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(li, retryBrokenPromise(coord.getLeader, GetLeaderRequest(key, info->present() ? info->get().changeID : UID()), TaskPriority::CoordinationReply));
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 2;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 1, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_2 = Future<Void>(Void());
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 3;
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 2, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
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
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state > 0) static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorNomineeActor*>(this)->ActorCallback< MonitorNomineeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorNomineeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state > 0) static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorNomineeActor*>(this)->ActorCallback< MonitorNomineeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MonitorNomineeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetLeaderReply") .suppressFor(1.0) .detail("Coordinator", coord.hostname.present() ? coord.hostname.get().toString() : coord.getLeader.getEndpoint().getPrimaryAddress().toString()) .detail("Nominee", li.present() ? li.get().changeID : UID()) .detail("ClusterKey", key.printable());
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (li != *info)
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			*info = li;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nomineeChange->trigger();
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (li.present() && li.get().forward)
															#line 1919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_3 = Future<Void>(Never());
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
				static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 4;
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 3, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetLeaderReply") .suppressFor(1.0) .detail("Coordinator", coord.hostname.present() ? coord.hostname.get().toString() : coord.getLeader.getEndpoint().getPrimaryAddress().toString()) .detail("Nominee", li.present() ? li.get().changeID : UID()) .detail("ClusterKey", key.printable());
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (li != *info)
															#line 1951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			*info = li;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nomineeChange->trigger();
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (li.present() && li.get().forward)
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_3 = Future<Void>(Never());
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
				static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 4;
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 3, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state > 0) static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorNomineeActor*>(this)->ActorCallback< MonitorNomineeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< MonitorNomineeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state > 0) static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorNomineeActor*>(this)->ActorCallback< MonitorNomineeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< MonitorNomineeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key key;
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientLeaderRegInterface coord;
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncTrigger* nomineeChange;
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<LeaderInfo>* info;
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<LeaderInfo> li;
															#line 2145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorNominee()
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorNomineeActor final : public Actor<Void>, public ActorCallback< MonitorNomineeActor, 0, Void >, public ActorCallback< MonitorNomineeActor, 1, Void >, public ActorCallback< MonitorNomineeActor, 2, Void >, public ActorCallback< MonitorNomineeActor, 3, Void >, public FastAllocated<MonitorNomineeActor>, public MonitorNomineeActorState<MonitorNomineeActor> {
															#line 2150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorNomineeActor>::operator new;
	using FastAllocated<MonitorNomineeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorNomineeActor, 0, Void >;
friend struct ActorCallback< MonitorNomineeActor, 1, Void >;
friend struct ActorCallback< MonitorNomineeActor, 2, Void >;
friend struct ActorCallback< MonitorNomineeActor, 3, Void >;
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorNomineeActor(Key const& key,ClientLeaderRegInterface const& coord,AsyncTrigger* const& nomineeChange,Optional<LeaderInfo>* const& info) 
															#line 2164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   MonitorNomineeActorState<MonitorNomineeActor>(key, coord, nomineeChange, info)
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitorNominee");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorNomineeActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MonitorNomineeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< MonitorNomineeActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< MonitorNomineeActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorNominee( Key const& key, ClientLeaderRegInterface const& coord, AsyncTrigger* const& nomineeChange, Optional<LeaderInfo>* const& info ) {
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorNomineeActor(key, coord, nomineeChange, info));
															#line 2195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

// Also used in fdbserver/LeaderElection.actor.cpp!
// bool represents if the LeaderInfo is a majority answer or not.
// This function also masks the first 7 bits of changeId of the nominees and returns the Leader with masked changeId
Optional<std::pair<LeaderInfo, bool>> getLeader(const std::vector<Optional<LeaderInfo>>& nominees) {
	// If any coordinator says that the quorum is forwarded, then it is
	for (int i = 0; i < nominees.size(); i++)
		if (nominees[i].present() && nominees[i].get().forward)
			return std::pair<LeaderInfo, bool>(nominees[i].get(), true);

	std::vector<std::pair<UID, int>> maskedNominees;
	maskedNominees.reserve(nominees.size());
	for (int i = 0; i < nominees.size(); i++) {
		if (nominees[i].present()) {
			maskedNominees.emplace_back(
			    UID(nominees[i].get().changeID.first() & LeaderInfo::changeIDMask, nominees[i].get().changeID.second()),
			    i);
		}
	}

	if (!maskedNominees.size())
		return Optional<std::pair<LeaderInfo, bool>>();

	std::sort(maskedNominees.begin(),
	          maskedNominees.end(),
	          [](const std::pair<UID, int>& l, const std::pair<UID, int>& r) { return l.first < r.first; });

	int bestCount = 1;
	int bestIdx = 0;
	int currentIdx = 0;
	int curCount = 1;
	for (int i = 1; i < maskedNominees.size(); i++) {
		if (maskedNominees[currentIdx].first == maskedNominees[i].first) {
			curCount++;
		} else {
			currentIdx = i;
			curCount = 1;
		}
		if (curCount > bestCount) {
			bestIdx = currentIdx;
			bestCount = curCount;
		}
	}

	bool majority = bestCount >= nominees.size() / 2 + 1;
	return std::pair<LeaderInfo, bool>(nominees[maskedNominees[bestIdx].second].get(), majority);
}

// Leader is the process that will be elected by coordinators as the cluster controller
															#line 2248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorLeaderOneGeneration()
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorLeaderOneGenerationActor>
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderOneGenerationActorState {
															#line 2255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderOneGenerationActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo,MonitorLeaderInfo const& info) 
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   outSerializedLeaderInfo(outSerializedLeaderInfo),
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(info),
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinators(info.intermediateConnRecord),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nomineeChange(),
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nominees(),
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allActors()
															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this));

	}
	~MonitorLeaderOneGenerationActorState() 
	{
		fdb_probe_actor_destroy("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nominees.resize(coordinators.clientLeaderServers.size());
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors = std::vector<Future<Void>>();
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors.reserve(coordinators.clientLeaderServers.size());
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for(int i = 0;i < coordinators.clientLeaderServers.size();i++) {
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				actors.push_back( monitorNominee(coordinators.clusterKey, coordinators.clientLeaderServers[i], &nomineeChange, &nominees[i]));
															#line 2297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			allActors = waitForAll(actors);
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 2303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~MonitorLeaderOneGenerationActorState();
		static_cast<MonitorLeaderOneGenerationActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		Optional<std::pair<LeaderInfo, bool>> leader = getLeader(nominees);
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("MonitorLeaderChange") .detail("NewLeader", leader.present() ? leader.get().first.changeID : UID(1, 1));
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (leader.present())
															#line 2337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (leader.get().first.forward)
															#line 2341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("MonitorLeaderForwarding") .detail("NewConnStr", leader.get().first.serializedInfo.toString()) .detail("OldConnStr", info.intermediateConnRecord->getConnectionString().toString()) .trackLatest("MonitorLeaderForwarding");
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				info.intermediateConnRecord = connRecord->makeIntermediateRecord( ClusterConnectionString(leader.get().first.serializedInfo.toString()));
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!static_cast<MonitorLeaderOneGenerationActor*>(this)->SAV<MonitorLeaderInfo>::futures) { (void)(info); this->~MonitorLeaderOneGenerationActorState(); static_cast<MonitorLeaderOneGenerationActor*>(this)->destroy(); return 0; }
															#line 2349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				new (&static_cast<MonitorLeaderOneGenerationActor*>(this)->SAV< MonitorLeaderInfo >::value()) MonitorLeaderInfo(std::move(info)); // state_var_RVO
				this->~MonitorLeaderOneGenerationActorState();
				static_cast<MonitorLeaderOneGenerationActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (connRecord != info.intermediateConnRecord)
															#line 2357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!info.hasConnected)
															#line 2361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					TraceEvent(SevWarnAlways, "IncorrectClusterFileContentsAtConnection") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", connRecord->getConnectionString().toString()) .detail("CurrentConnectionString", info.intermediateConnRecord->getConnectionString().toString());
															#line 2365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				connRecord->setAndPersistConnectionString(info.intermediateConnRecord->getConnectionString());
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				info.intermediateConnRecord = connRecord;
															#line 2371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			info.hasConnected = true;
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			connRecord->notifyConnected();
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outSerializedLeaderInfo->set(leader.get().first.serializedInfo);
															#line 2379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = nomineeChange.onTrigger() || allActors;
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state = 1;
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >*>(static_cast<MonitorLeaderOneGenerationActor*>(this)));
															#line 2390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorLeaderOneGenerationActor*>(this)->ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Value>> outSerializedLeaderInfo;
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientCoordinators coordinators;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncTrigger nomineeChange;
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Optional<LeaderInfo>> nominees;
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<Void> allActors;
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Future<Void>> actors;
															#line 2486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorLeaderOneGeneration()
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderOneGenerationActor final : public Actor<MonitorLeaderInfo>, public ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >, public FastAllocated<MonitorLeaderOneGenerationActor>, public MonitorLeaderOneGenerationActorState<MonitorLeaderOneGenerationActor> {
															#line 2491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorLeaderOneGenerationActor>::operator new;
	using FastAllocated<MonitorLeaderOneGenerationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<MonitorLeaderInfo>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >;
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderOneGenerationActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo,MonitorLeaderInfo const& info) 
															#line 2502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<MonitorLeaderInfo>(),
		   MonitorLeaderOneGenerationActorState<MonitorLeaderOneGenerationActor>(connRecord, outSerializedLeaderInfo, info)
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitorLeaderOneGeneration");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<MonitorLeaderInfo> monitorLeaderOneGeneration( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<Value>> const& outSerializedLeaderInfo, MonitorLeaderInfo const& info ) {
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<MonitorLeaderInfo>(new MonitorLeaderOneGenerationActor(connRecord, outSerializedLeaderInfo, info));
															#line 2530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 2535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorLeaderInternal()
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorLeaderInternalActor>
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderInternalActorState {
															#line 2542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInternalActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo) 
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   outSerializedLeaderInfo(outSerializedLeaderInfo),
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(connRecord)
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("monitorLeaderInternal", reinterpret_cast<unsigned long>(this));

	}
	~MonitorLeaderInternalActorState() 
	{
		fdb_probe_actor_destroy("monitorLeaderInternal", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 2568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~MonitorLeaderInternalActorState();
		static_cast<MonitorLeaderInternalActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<MonitorLeaderInfo> __when_expr_0 = monitorLeaderOneGeneration(connRecord, outSerializedLeaderInfo, info);
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorLeaderInternalActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorLeaderInternalActor*>(this)->actor_wait_state = 1;
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >*>(static_cast<MonitorLeaderInternalActor*>(this)));
															#line 2605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(MonitorLeaderInfo const& _info,int loopDepth) 
	{
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 2614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(MonitorLeaderInfo && _info,int loopDepth) 
	{
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 2623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(MonitorLeaderInfo const& _info,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_info, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(MonitorLeaderInfo && _info,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_info), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MonitorLeaderInternalActor*>(this)->actor_wait_state > 0) static_cast<MonitorLeaderInternalActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorLeaderInternalActor*>(this)->ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >*,MonitorLeaderInfo const& value) 
	{
		fdb_probe_actor_enter("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >*,MonitorLeaderInfo && value) 
	{
		fdb_probe_actor_enter("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >*,Error err) 
	{
		fdb_probe_actor_enter("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Value>> outSerializedLeaderInfo;
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorLeaderInternal()
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderInternalActor final : public Actor<Void>, public ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >, public FastAllocated<MonitorLeaderInternalActor>, public MonitorLeaderInternalActorState<MonitorLeaderInternalActor> {
															#line 2702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorLeaderInternalActor>::operator new;
	using FastAllocated<MonitorLeaderInternalActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >;
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInternalActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo) 
															#line 2713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   MonitorLeaderInternalActorState<MonitorLeaderInternalActor>(connRecord, outSerializedLeaderInfo)
	{
		fdb_probe_actor_enter("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitorLeaderInternal");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitorLeaderInternal", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorLeaderInternal( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<Value>> const& outSerializedLeaderInfo ) {
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorLeaderInternalActor(connRecord, outSerializedLeaderInfo));
															#line 2741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 2746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via asyncDeserializeClusterInterface()
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class AsyncDeserializeClusterInterfaceActor>
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class AsyncDeserializeClusterInterfaceActorState {
															#line 2753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncDeserializeClusterInterfaceActorState(Reference<AsyncVar<Value>> const& serializedInfo,Reference<AsyncVar<Optional<ClusterInterface>>> const& outKnownLeader) 
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : serializedInfo(serializedInfo),
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   outKnownLeader(outKnownLeader),
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   knownLeader(new AsyncVar<Optional<ClusterControllerClientInterface>>{}),
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   deserializer(asyncDeserialize(serializedInfo, knownLeader))
															#line 2766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this));

	}
	~AsyncDeserializeClusterInterfaceActorState() 
	{
		fdb_probe_actor_destroy("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~AsyncDeserializeClusterInterfaceActorState();
		static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = deserializer;
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_1 = knownLeader->onChange();
															#line 2817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->actor_wait_state = 1;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >*>(static_cast<AsyncDeserializeClusterInterfaceActor*>(this)));
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >*>(static_cast<AsyncDeserializeClusterInterfaceActor*>(this)));
															#line 2824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		UNSTOPPABLE_ASSERT(false);
															#line 2839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		UNSTOPPABLE_ASSERT(false);
															#line 2848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (knownLeader->get().present())
															#line 2857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(knownLeader->get().get().clientInterface);
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(Optional<ClusterInterface>{});
															#line 2867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (knownLeader->get().present())
															#line 2877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(knownLeader->get().get().clientInterface);
															#line 2881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(Optional<ClusterInterface>{});
															#line 2887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->actor_wait_state > 0) static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >::remove();
		static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Value>> serializedInfo;
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterInterface>>> outKnownLeader;
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> knownLeader;
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<Void> deserializer;
															#line 2998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via asyncDeserializeClusterInterface()
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class AsyncDeserializeClusterInterfaceActor final : public Actor<Void>, public ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >, public ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >, public FastAllocated<AsyncDeserializeClusterInterfaceActor>, public AsyncDeserializeClusterInterfaceActorState<AsyncDeserializeClusterInterfaceActor> {
															#line 3003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<AsyncDeserializeClusterInterfaceActor>::operator new;
	using FastAllocated<AsyncDeserializeClusterInterfaceActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >;
friend struct ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >;
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncDeserializeClusterInterfaceActor(Reference<AsyncVar<Value>> const& serializedInfo,Reference<AsyncVar<Optional<ClusterInterface>>> const& outKnownLeader) 
															#line 3015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   AsyncDeserializeClusterInterfaceActorState<AsyncDeserializeClusterInterfaceActor>(serializedInfo, outKnownLeader)
	{
		fdb_probe_actor_enter("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("asyncDeserializeClusterInterface");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("asyncDeserializeClusterInterface", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> asyncDeserializeClusterInterface( Reference<AsyncVar<Value>> const& serializedInfo, Reference<AsyncVar<Optional<ClusterInterface>>> const& outKnownLeader ) {
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new AsyncDeserializeClusterInterfaceActor(serializedInfo, outKnownLeader));
															#line 3043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

struct ClientStatusStats {
	int count;
	std::vector<std::pair<NetworkAddress, Key>> examples;

	ClientStatusStats() : count(0) { examples.reserve(CLIENT_KNOBS->CLIENT_EXAMPLE_AMOUNT); }
};

OpenDatabaseRequest ClientData::getRequest() {
	OpenDatabaseRequest req;

	std::map<StringRef, ClientStatusStats> issueMap;
	std::map<ClientVersionRef, ClientStatusStats> versionMap;
	std::map<StringRef, ClientStatusStats> maxProtocolMap;
	int clientCount = 0;

	// SOMEDAY: add a yield in this loop
	for (auto& ci : clientStatusInfoMap) {
		for (auto& it : ci.second.issues) {
			auto& entry = issueMap[it];
			entry.count++;
			if (entry.examples.size() < CLIENT_KNOBS->CLIENT_EXAMPLE_AMOUNT) {
				entry.examples.emplace_back(ci.first, ci.second.traceLogGroup);
			}
		}
		if (ci.second.versions.size()) {
			clientCount++;
			StringRef maxProtocol;
			for (auto& it : ci.second.versions) {
				maxProtocol = std::max(maxProtocol, it.protocolVersion);
				auto& entry = versionMap[it];
				entry.count++;
				if (entry.examples.size() < CLIENT_KNOBS->CLIENT_EXAMPLE_AMOUNT) {
					entry.examples.emplace_back(ci.first, ci.second.traceLogGroup);
				}
			}
			auto& maxEntry = maxProtocolMap[maxProtocol];
			maxEntry.count++;
			if (maxEntry.examples.size() < CLIENT_KNOBS->CLIENT_EXAMPLE_AMOUNT) {
				maxEntry.examples.emplace_back(ci.first, ci.second.traceLogGroup);
			}
		} else {
			auto& entry = versionMap[ClientVersionRef()];
			entry.count++;
			if (entry.examples.size() < CLIENT_KNOBS->CLIENT_EXAMPLE_AMOUNT) {
				entry.examples.emplace_back(ci.first, ci.second.traceLogGroup);
			}
		}
	}

	req.issues.reserve(issueMap.size());
	for (auto& it : issueMap) {
		req.issues.push_back(ItemWithExamples<Key>(it.first, it.second.count, it.second.examples));
	}
	req.supportedVersions.reserve(versionMap.size());
	for (auto& it : versionMap) {
		req.supportedVersions.push_back(
		    ItemWithExamples<Standalone<ClientVersionRef>>(it.first, it.second.count, it.second.examples));
	}
	req.maxProtocolSupported.reserve(maxProtocolMap.size());
	for (auto& it : maxProtocolMap) {
		req.maxProtocolSupported.push_back(ItemWithExamples<Key>(it.first, it.second.count, it.second.examples));
	}
	req.clientCount = clientCount;

	return req;
}

															#line 3115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via getClientInfoFromLeader()
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class GetClientInfoFromLeaderActor>
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class GetClientInfoFromLeaderActorState {
															#line 3122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	GetClientInfoFromLeaderActorState(Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> const& knownLeader,ClientData* const& clientData) 
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : knownLeader(knownLeader),
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientData(clientData)
															#line 3131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this));

	}
	~GetClientInfoFromLeaderActorState() 
	{
		fdb_probe_actor_destroy("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 3146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~GetClientInfoFromLeaderActorState();
		static_cast<GetClientInfoFromLeaderActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		lastRequestTime = now();
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req = clientData->getRequest();
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		;
															#line 3173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!(!knownLeader->get().present()))
															#line 3189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = knownLeader->onChange();
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state = 1;
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetClientInfoFromLeaderActor, 0, Void >*>(static_cast<GetClientInfoFromLeaderActor*>(this)));
															#line 3202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state > 0) static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state = 0;
		static_cast<GetClientInfoFromLeaderActor*>(this)->ActorCallback< GetClientInfoFromLeaderActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetClientInfoFromLeaderActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetClientInfoFromLeaderActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetClientInfoFromLeaderActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (now() - lastRequestTime > CLIENT_KNOBS->MAX_CLIENT_STATUS_AGE)
															#line 3306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			lastRequestTime = now();
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			req = clientData->getRequest();
															#line 3312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			resetReply(req);
															#line 3318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.knownClientInfoID = clientData->clientInfo->get().read().id;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<ClientDBInfo> __when_expr_1 = brokenPromiseToNever(knownLeader->get().get().clientInterface.openDatabase.getReply(req));
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_2 = knownLeader->onChange();
															#line 3330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when2(__when_expr_2.get(), loopDepth); };
		static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state = 2;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >*>(static_cast<GetClientInfoFromLeaderActor*>(this)));
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetClientInfoFromLeaderActor, 2, Void >*>(static_cast<GetClientInfoFromLeaderActor*>(this)));
															#line 3337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(ClientDBInfo const& ni,int loopDepth) 
	{
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetClientInfoFromLeaderGotClientInfo", knownLeader->get().get().clientInterface.id()) .detail("CommitProxy0", ni.commitProxies.size() ? ni.commitProxies[0].address().toString() : "") .detail("GrvProxy0", ni.grvProxies.size() ? ni.grvProxies[0].address().toString() : "") .detail("ClientID", ni.id);
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientData->clientInfo->set(CachedSerialization<ClientDBInfo>(ni));
															#line 3354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(ClientDBInfo && ni,int loopDepth) 
	{
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetClientInfoFromLeaderGotClientInfo", knownLeader->get().get().clientInterface.id()) .detail("CommitProxy0", ni.commitProxies.size() ? ni.commitProxies[0].address().toString() : "") .detail("GrvProxy0", ni.grvProxies.size() ? ni.grvProxies[0].address().toString() : "") .detail("ClientID", ni.id);
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientData->clientInfo->set(CachedSerialization<ClientDBInfo>(ni));
															#line 3365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state > 0) static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state = 0;
		static_cast<GetClientInfoFromLeaderActor*>(this)->ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >::remove();
		static_cast<GetClientInfoFromLeaderActor*>(this)->ActorCallback< GetClientInfoFromLeaderActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >*,ClientDBInfo const& value) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >*,ClientDBInfo && value) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >*,Error err) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetClientInfoFromLeaderActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetClientInfoFromLeaderActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetClientInfoFromLeaderActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> knownLeader;
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientData* clientData;
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	double lastRequestTime;
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	OpenDatabaseRequest req;
															#line 3487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via getClientInfoFromLeader()
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class GetClientInfoFromLeaderActor final : public Actor<Void>, public ActorCallback< GetClientInfoFromLeaderActor, 0, Void >, public ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >, public ActorCallback< GetClientInfoFromLeaderActor, 2, Void >, public FastAllocated<GetClientInfoFromLeaderActor>, public GetClientInfoFromLeaderActorState<GetClientInfoFromLeaderActor> {
															#line 3492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<GetClientInfoFromLeaderActor>::operator new;
	using FastAllocated<GetClientInfoFromLeaderActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetClientInfoFromLeaderActor, 0, Void >;
friend struct ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >;
friend struct ActorCallback< GetClientInfoFromLeaderActor, 2, Void >;
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	GetClientInfoFromLeaderActor(Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> const& knownLeader,ClientData* const& clientData) 
															#line 3505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   GetClientInfoFromLeaderActorState<GetClientInfoFromLeaderActor>(knownLeader, clientData)
	{
		fdb_probe_actor_enter("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getClientInfoFromLeader");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getClientInfoFromLeader", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetClientInfoFromLeaderActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> getClientInfoFromLeader( Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> const& knownLeader, ClientData* const& clientData ) {
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new GetClientInfoFromLeaderActor(knownLeader, clientData));
															#line 3534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 3539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorLeaderAndGetClientInfo()
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorLeaderAndGetClientInfoActor>
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderAndGetClientInfoActorState {
															#line 3546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderAndGetClientInfoActorState(Key const& clusterKey,std::vector<Hostname> const& hostnames,std::vector<NetworkAddress> const& coordinators,ClientData* const& clientData,Reference<AsyncVar<Optional<LeaderInfo>>> const& leaderInfo) 
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : clusterKey(clusterKey),
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   hostnames(hostnames),
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinators(coordinators),
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientData(clientData),
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   leaderInfo(leaderInfo),
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientLeaderServers(),
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nomineeChange(),
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nominees(),
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allActors(),
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   knownLeader(new AsyncVar<Optional<ClusterControllerClientInterface>>{})
															#line 3571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this));

	}
	~MonitorLeaderAndGetClientInfoActorState() 
	{
		fdb_probe_actor_destroy("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			clientLeaderServers.reserve(hostnames.size() + coordinators.size());
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( auto h : hostnames ) {
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(h));
															#line 3590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( auto s : coordinators ) {
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(s));
															#line 3596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nominees.resize(clientLeaderServers.size());
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::vector<Future<Void>> actors;
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors.reserve(clientLeaderServers.size());
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for(int i = 0;i < clientLeaderServers.size();i++) {
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				actors.push_back(monitorNominee(clusterKey, clientLeaderServers[i], &nomineeChange, &nominees[i]));
															#line 3608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors.push_back(getClientInfoFromLeader(knownLeader, clientData));
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			allActors = waitForAll(actors);
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 3616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~MonitorLeaderAndGetClientInfoActorState();
		static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		Optional<std::pair<LeaderInfo, bool>> leader = getLeader(nominees);
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("MonitorLeaderAndGetClientInfoLeaderChange") .detail("NewLeader", leader.present() ? leader.get().first.changeID : UID(1, 1)) .detail("Key", clusterKey.printable());
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (leader.present())
															#line 3650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (leader.get().first.forward)
															#line 3654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClientDBInfo outInfo;
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				outInfo.id = deterministicRandom()->randomUniqueID();
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				outInfo.forward = leader.get().first.serializedInfo;
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientData->clientInfo->set(CachedSerialization<ClientDBInfo>(outInfo));
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				leaderInfo->set(leader.get().first);
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("MonitorLeaderAndGetClientInfoForwarding") .detail("NewConnStr", leader.get().first.serializedInfo.toString());
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MonitorLeaderAndGetClientInfoActorState(); static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->destroy(); return 0; }
															#line 3670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				new (&static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~MonitorLeaderAndGetClientInfoActorState();
				static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (leader.get().first.serializedInfo.size())
															#line 3678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ObjectReader reader(leader.get().first.serializedInfo.begin(), IncludeVersion());
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterControllerClientInterface res;
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				reader.deserialize(res);
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				knownLeader->set(res);
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				leaderInfo->set(leader.get().first);
															#line 3690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
		}
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = nomineeChange.onTrigger() || allActors;
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->actor_wait_state = 1;
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >*>(static_cast<MonitorLeaderAndGetClientInfoActor*>(this)));
															#line 3702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->actor_wait_state > 0) static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key clusterKey;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Hostname> hostnames;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<NetworkAddress> coordinators;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientData* clientData;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<LeaderInfo>>> leaderInfo;
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<ClientLeaderRegInterface> clientLeaderServers;
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncTrigger nomineeChange;
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Optional<LeaderInfo>> nominees;
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<Void> allActors;
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> knownLeader;
															#line 3802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorLeaderAndGetClientInfo()
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderAndGetClientInfoActor final : public Actor<Void>, public ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >, public FastAllocated<MonitorLeaderAndGetClientInfoActor>, public MonitorLeaderAndGetClientInfoActorState<MonitorLeaderAndGetClientInfoActor> {
															#line 3807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorLeaderAndGetClientInfoActor>::operator new;
	using FastAllocated<MonitorLeaderAndGetClientInfoActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderAndGetClientInfoActor(Key const& clusterKey,std::vector<Hostname> const& hostnames,std::vector<NetworkAddress> const& coordinators,ClientData* const& clientData,Reference<AsyncVar<Optional<LeaderInfo>>> const& leaderInfo) 
															#line 3818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   MonitorLeaderAndGetClientInfoActorState<MonitorLeaderAndGetClientInfoActor>(clusterKey, hostnames, coordinators, clientData, leaderInfo)
	{
		fdb_probe_actor_enter("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitorLeaderAndGetClientInfo");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitorLeaderAndGetClientInfo", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorLeaderAndGetClientInfo( Key const& clusterKey, std::vector<Hostname> const& hostnames, std::vector<NetworkAddress> const& coordinators, ClientData* const& clientData, Reference<AsyncVar<Optional<LeaderInfo>>> const& leaderInfo ) {
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorLeaderAndGetClientInfoActor(clusterKey, hostnames, coordinators, clientData, leaderInfo));
															#line 3846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

void shrinkProxyList(ClientDBInfo& ni,
                     std::vector<UID>& lastCommitProxyUIDs,
                     std::vector<CommitProxyInterface>& lastCommitProxies,
                     std::vector<UID>& lastGrvProxyUIDs,
                     std::vector<GrvProxyInterface>& lastGrvProxies) {
	if (ni.commitProxies.size() > CLIENT_KNOBS->MAX_COMMIT_PROXY_CONNECTIONS) {
		std::vector<UID> commitProxyUIDs;
		for (auto& commitProxy : ni.commitProxies) {
			commitProxyUIDs.push_back(commitProxy.id());
		}
		if (commitProxyUIDs != lastCommitProxyUIDs) {
			lastCommitProxyUIDs.swap(commitProxyUIDs);
			lastCommitProxies = ni.commitProxies;
			deterministicRandom()->randomShuffle(lastCommitProxies);
			lastCommitProxies.resize(CLIENT_KNOBS->MAX_COMMIT_PROXY_CONNECTIONS);
			for (int i = 0; i < lastCommitProxies.size(); i++) {
				TraceEvent("ConnectedCommitProxy").detail("CommitProxy", lastCommitProxies[i].id());
			}
		}
		ni.firstCommitProxy = ni.commitProxies[0];
		ni.commitProxies = lastCommitProxies;
	}
	if (ni.grvProxies.size() > CLIENT_KNOBS->MAX_GRV_PROXY_CONNECTIONS) {
		std::vector<UID> grvProxyUIDs;
		for (auto& grvProxy : ni.grvProxies) {
			grvProxyUIDs.push_back(grvProxy.id());
		}
		if (grvProxyUIDs != lastGrvProxyUIDs) {
			lastGrvProxyUIDs.swap(grvProxyUIDs);
			lastGrvProxies = ni.grvProxies;
			deterministicRandom()->randomShuffle(lastGrvProxies);
			lastGrvProxies.resize(CLIENT_KNOBS->MAX_GRV_PROXY_CONNECTIONS);
			for (int i = 0; i < lastGrvProxies.size(); i++) {
				TraceEvent("ConnectedGrvProxy").detail("GrvProxy", lastGrvProxies[i].id());
			}
		}
		ni.grvProxies = lastGrvProxies;
	}
}

															#line 3891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorProxiesOneGeneration()
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorProxiesOneGenerationActor>
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesOneGenerationActorState {
															#line 3898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesOneGenerationActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,MonitorLeaderInfo const& info,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientInfo(clientInfo),
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinator(coordinator),
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(info),
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   supportedVersions(supportedVersions),
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   traceLogGroup(traceLogGroup),
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   internal(internal),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   cs(info.intermediateConnRecord->getConnectionString()),
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinatorsSize(cs.hostnames.size() + cs.coords.size()),
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   index(0),
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   successIndex(0),
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   incorrectTime(),
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastCommitProxyUIDs(),
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastCommitProxies(),
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastGrvProxyUIDs(),
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastGrvProxies(),
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientLeaderServers()
															#line 3937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this));

	}
	~MonitorProxiesOneGenerationActorState() 
	{
		fdb_probe_actor_destroy("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			clientLeaderServers.reserve(coordinatorsSize);
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( const auto& h : cs.hostnames ) {
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(h));
															#line 3956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( const auto& c : cs.coords ) {
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(c));
															#line 3962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			deterministicRandom()->randomShuffle(clientLeaderServers);
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 3968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~MonitorProxiesOneGenerationActorState();
		static_cast<MonitorProxiesOneGenerationActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientLeaderServer = clientLeaderServers[index];
															#line 890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req = OpenDatabaseCoordRequest();
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.clusterKey = cs.clusterKey();
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.hostnames = cs.hostnames;
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.coordinators = cs.coords;
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.knownClientInfoID = clientInfo->get().id;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.supportedVersions = supportedVersions->get();
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.traceLogGroup = traceLogGroup;
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.internal = internal;
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		storedConnectionString = ClusterConnectionString();
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (connRecord)
															#line 4018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<bool> __when_expr_0 = connRecord->upToDate(storedConnectionString);
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 1;
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			incorrectTime = Optional<double>();
															#line 4036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		repFuture = Future<ErrorOr<CachedSerialization<ClientDBInfo>>>();
															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (clientLeaderServer.hostname.present())
															#line 4048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			repFuture = tryGetReplyFromHostname(req, clientLeaderServer.hostname.get(), WLTOKEN_CLIENTLEADERREG_OPENDATABASE, TaskPriority::CoordinationReply);
															#line 4052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			repFuture = clientLeaderServer.openDatabase.tryGetReply(req, TaskPriority::CoordinationReply);
															#line 4058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		coordinator->setUnconditional(clientLeaderServer);
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<ErrorOr<CachedSerialization<ClientDBInfo>>> __when_expr_1 = repFuture;
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 2;
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& upToDate,int loopDepth) 
	{
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!upToDate)
															#line 4080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			req.issues.push_back_deep(req.issues.arena(), LiteralStringRef("incorrect_cluster_file_contents"));
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = connRecord->getConnectionString().toString();
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (!incorrectTime.present())
															#line 4088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				incorrectTime = now();
															#line 4092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			TraceEvent(now() - incorrectTime.get() > 300 ? SevWarnAlways : SevWarn, "IncorrectClusterFileContents") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", storedConnectionString.toString()) .detail("CurrentConnectionString", connectionString);
															#line 4096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			incorrectTime = Optional<double>();
															#line 4102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && upToDate,int loopDepth) 
	{
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!upToDate)
															#line 4112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			req.issues.push_back_deep(req.issues.arena(), LiteralStringRef("incorrect_cluster_file_contents"));
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = connRecord->getConnectionString().toString();
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (!incorrectTime.present())
															#line 4120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				incorrectTime = now();
															#line 4124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			TraceEvent(now() - incorrectTime.get() > 300 ? SevWarnAlways : SevWarn, "IncorrectClusterFileContents") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", storedConnectionString.toString()) .detail("CurrentConnectionString", connectionString);
															#line 4128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			incorrectTime = Optional<double>();
															#line 4134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& upToDate,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(upToDate, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && upToDate,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(upToDate), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont8(int loopDepth) 
	{
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (rep.present())
															#line 4207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (rep.get().read().forward.present())
															#line 4211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("MonitorProxiesForwarding") .detail("NewConnStr", rep.get().read().forward.get().toString()) .detail("OldConnStr", info.intermediateConnRecord->getConnectionString().toString());
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				info.intermediateConnRecord = connRecord->makeIntermediateRecord( ClusterConnectionString(rep.get().read().forward.get().toString()));
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV<MonitorLeaderInfo>::futures) { (void)(info); this->~MonitorProxiesOneGenerationActorState(); static_cast<MonitorProxiesOneGenerationActor*>(this)->destroy(); return 0; }
															#line 4219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				new (&static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV< MonitorLeaderInfo >::value()) MonitorLeaderInfo(std::move(info)); // state_var_RVO
				this->~MonitorProxiesOneGenerationActorState();
				static_cast<MonitorProxiesOneGenerationActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (connRecord != info.intermediateConnRecord)
															#line 4227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!info.hasConnected)
															#line 4231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					TraceEvent(SevWarnAlways, "IncorrectClusterFileContentsAtConnection") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", connRecord->getConnectionString().toString()) .detail("CurrentConnectionString", info.intermediateConnRecord->getConnectionString().toString());
															#line 4235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				connRecord->setAndPersistConnectionString(info.intermediateConnRecord->getConnectionString());
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				info.intermediateConnRecord = connRecord;
															#line 4241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			info.hasConnected = true;
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			connRecord->notifyConnected();
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			auto& ni = rep.get().mutate();
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			shrinkProxyList(ni, lastCommitProxyUIDs, lastCommitProxies, lastGrvProxyUIDs, lastGrvProxies);
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			clientInfo->setUnconditional(ni);
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			successIndex = index;
															#line 4255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont11(loopDepth);
		}
		else
		{
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			TEST(rep.getError().code() == error_code_failed_to_progress);
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			TEST(rep.getError().code() == error_code_lookup_failed);
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			index = (index + 1) % coordinatorsSize;
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (index == successIndex)
															#line 4268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_2 = delay(CLIENT_KNOBS->COORDINATOR_RECONNECTION_DELAY);
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont8when1(__when_expr_2.get(), loopDepth); };
				static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 3;
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont16(loopDepth);
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<CachedSerialization<ClientDBInfo>> const& __rep,int loopDepth) 
	{
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		rep = __rep;
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<CachedSerialization<ClientDBInfo>> && __rep,int loopDepth) 
	{
		rep = std::move(__rep);
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >*,ErrorOr<CachedSerialization<ClientDBInfo>> const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >*,ErrorOr<CachedSerialization<ClientDBInfo>> && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont11(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont16(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont17(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont17(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont17(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont17(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<ClientDBInfo>> clientInfo;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> coordinator;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> supportedVersions;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key traceLogGroup;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	IsInternal internal;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClusterConnectionString cs;
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	int coordinatorsSize;
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	int index;
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	int successIndex;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<double> incorrectTime;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<UID> lastCommitProxyUIDs;
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<CommitProxyInterface> lastCommitProxies;
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<UID> lastGrvProxyUIDs;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<GrvProxyInterface> lastGrvProxies;
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<ClientLeaderRegInterface> clientLeaderServers;
															#line 889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientLeaderRegInterface clientLeaderServer;
															#line 890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	OpenDatabaseCoordRequest req;
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClusterConnectionString storedConnectionString;
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<ErrorOr<CachedSerialization<ClientDBInfo>>> repFuture;
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ErrorOr<CachedSerialization<ClientDBInfo>> rep;
															#line 4488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorProxiesOneGeneration()
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesOneGenerationActor final : public Actor<MonitorLeaderInfo>, public ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >, public ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >, public ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >, public FastAllocated<MonitorProxiesOneGenerationActor>, public MonitorProxiesOneGenerationActorState<MonitorProxiesOneGenerationActor> {
															#line 4493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorProxiesOneGenerationActor>::operator new;
	using FastAllocated<MonitorProxiesOneGenerationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<MonitorLeaderInfo>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >;
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >;
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesOneGenerationActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,MonitorLeaderInfo const& info,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 4506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<MonitorLeaderInfo>(),
		   MonitorProxiesOneGenerationActorState<MonitorProxiesOneGenerationActor>(connRecord, clientInfo, coordinator, info, supportedVersions, traceLogGroup, internal)
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitorProxiesOneGeneration");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 1, ErrorOr<CachedSerialization<ClientDBInfo>> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<MonitorLeaderInfo> monitorProxiesOneGeneration( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<ClientDBInfo>> const& clientInfo, Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator, MonitorLeaderInfo const& info, Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions, Key const& traceLogGroup, IsInternal const& internal ) {
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<MonitorLeaderInfo>(new MonitorProxiesOneGenerationActor(connRecord, clientInfo, coordinator, info, supportedVersions, traceLogGroup, internal));
															#line 4536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 4541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorProxies()
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorProxiesActor>
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesActorState {
															#line 4548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesActorState(Reference<AsyncVar<Reference<IClusterConnectionRecord>>> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientInfo(clientInfo),
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinator(coordinator),
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   supportedVersions(supportedVersions),
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   traceLogGroup(traceLogGroup),
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   internal(internal),
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(connRecord->get())
															#line 4567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("monitorProxies", reinterpret_cast<unsigned long>(this));

	}
	~MonitorProxiesActorState() 
	{
		fdb_probe_actor_destroy("monitorProxies", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 4582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~MonitorProxiesActorState();
		static_cast<MonitorProxiesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<MonitorLeaderInfo> __when_expr_0 = monitorProxiesOneGeneration( connRecord->get(), clientInfo, coordinator, info, supportedVersions, traceLogGroup, internal);
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorProxiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_1 = connRecord->onChange();
															#line 4618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<MonitorProxiesActor*>(this)->actor_wait_state = 1;
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >*>(static_cast<MonitorProxiesActor*>(this)));
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesActor, 1, Void >*>(static_cast<MonitorProxiesActor*>(this)));
															#line 4625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(MonitorLeaderInfo const& _info,int loopDepth) 
	{
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 4640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(MonitorLeaderInfo && _info,int loopDepth) 
	{
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 4649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.hasConnected = false;
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord->get();
															#line 4660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.hasConnected = false;
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord->get();
															#line 4671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MonitorProxiesActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesActor*>(this)->ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >::remove();
		static_cast<MonitorProxiesActor*>(this)->ActorCallback< MonitorProxiesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >*,MonitorLeaderInfo const& value) 
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >*,MonitorLeaderInfo && value) 
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MonitorProxiesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Reference<IClusterConnectionRecord>>> connRecord;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<ClientDBInfo>> clientInfo;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> coordinator;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> supportedVersions;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key traceLogGroup;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	IsInternal internal;
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 4787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorProxies()
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesActor final : public Actor<Void>, public ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >, public ActorCallback< MonitorProxiesActor, 1, Void >, public FastAllocated<MonitorProxiesActor>, public MonitorProxiesActorState<MonitorProxiesActor> {
															#line 4792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorProxiesActor>::operator new;
	using FastAllocated<MonitorProxiesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >;
friend struct ActorCallback< MonitorProxiesActor, 1, Void >;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesActor(Reference<AsyncVar<Reference<IClusterConnectionRecord>>> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 4804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   MonitorProxiesActorState<MonitorProxiesActor>(connRecord, clientInfo, coordinator, supportedVersions, traceLogGroup, internal)
	{
		fdb_probe_actor_enter("monitorProxies", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitorProxies");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitorProxies", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorProxies( Reference<AsyncVar<Reference<IClusterConnectionRecord>>> const& connRecord, Reference<AsyncVar<ClientDBInfo>> const& clientInfo, Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator, Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions, Key const& traceLogGroup, IsInternal const& internal ) {
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorProxiesActor(connRecord, clientInfo, coordinator, supportedVersions, traceLogGroup, internal));
															#line 4832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
