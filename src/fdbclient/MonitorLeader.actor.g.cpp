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
#include "flow/IConnection.h"
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

															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase128()
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase128Actor>
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase128ActorState {
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase128ActorState(UnitTestParameters const& params) 
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   input()
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase128", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase128ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase128", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@1.1.1.1:345";
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(input);
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(input == cs.toString());
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
			{
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@1.1.1.1:345,1.1.1.1:345";
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				try {
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					ClusterConnectionString cs(input);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~FlowTestCase128ActorState();
		static_cast<FlowTestCase128Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@1.1.1.1:34534,5.1.5.3:23443";
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(input);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@1.1.1.1:34534,5.1.5.3:23443";
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@[::1]:1234,[::1]:1235";
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@[abcd:dcba::1]:1234,[abcd:dcba::abcd:1]:1234";
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase128Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase128ActorState(); static_cast<FlowTestCase128Actor*>(this)->destroy(); return 0; }
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase128Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase128ActorState();
		static_cast<FlowTestCase128Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(e.code() == error_code_connection_string_invalid);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::string input;
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase128()
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase128Actor final : public Actor<Void>, public FastAllocated<FlowTestCase128Actor>, public FlowTestCase128ActorState<FlowTestCase128Actor> {
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase128Actor>::operator new;
	using FastAllocated<FlowTestCase128Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase128Actor(UnitTestParameters const& params) 
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase128ActorState<FlowTestCase128Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase128", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase128");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase128", reinterpret_cast<unsigned long>(this), -1);

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
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase128( UnitTestParameters const& params ) {
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase128Actor(params));
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase128, "/fdbclient/MonitorLeader/parseConnectionString/addresses")

#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase188()
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase188Actor>
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase188ActorState {
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase188ActorState(UnitTestParameters const& params) 
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params),
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   input()
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase188", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase188ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase188", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@localhost:1234";
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(input);
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.hostnames.size() == 1);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(input == cs.toString());
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
			{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				input = "asdf:2345@localhost:1234,localhost:1234";
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				try {
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					ClusterConnectionString cs(input);
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~FlowTestCase188ActorState();
		static_cast<FlowTestCase188Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@localhost:34534,host-name:23443";
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(input);
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(cs.hostnames.size() == 2);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@localhost:34534,host-name:23443";
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(cs.hostnames.size() == 2);
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		{
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			input = "0xxdeadbeef:100100100@localhost:34534,host-name_part1.host-name_part2:1234:tls";
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string commented("#start of comment\n");
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += input;
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "\n";
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			commented += "# asdfasdf ##";
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(commented);
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(cs.hostnames.size() == 2);
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(input == cs.toString());
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase188Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase188ActorState(); static_cast<FlowTestCase188Actor*>(this)->destroy(); return 0; }
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase188Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase188ActorState();
		static_cast<FlowTestCase188Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(e.code() == error_code_connection_string_invalid);
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::string input;
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase188()
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase188Actor final : public Actor<Void>, public FastAllocated<FlowTestCase188Actor>, public FlowTestCase188ActorState<FlowTestCase188Actor> {
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase188Actor>::operator new;
	using FastAllocated<FlowTestCase188Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase188Actor(UnitTestParameters const& params) 
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase188ActorState<FlowTestCase188Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase188", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase188");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase188", reinterpret_cast<unsigned long>(this), -1);

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
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase188( UnitTestParameters const& params ) {
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase188Actor(params));
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase188, "/fdbclient/MonitorLeader/parseConnectionString/hostnames")

#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase241()
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase241Actor>
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase241ActorState {
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase241ActorState(UnitTestParameters const& params) 
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase241", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase241ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase241", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = "TestCluster:0@localhost:1234,host-name:5678";
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string hn1 = "localhost", port1 = "1234", hn2 = "host-name", port2 = "5678";
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::vector<Hostname> hostnames;
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn1 + ":" + port1));
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn2 + ":" + port2));
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(hostnames, "TestCluster:0"_sr);
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.hostnames.size() == 2);
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.coords.size() == 0);
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(cs.toString() == connectionString);
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
			{
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.clear();
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn1 + ":" + port1));
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				hostnames.push_back(Hostname::parse(hn1 + ":" + port1));
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				try {
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					ClusterConnectionString cs(hostnames, "TestCluster:0"_sr);
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~FlowTestCase241ActorState();
		static_cast<FlowTestCase241Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase241Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase241ActorState(); static_cast<FlowTestCase241Actor*>(this)->destroy(); return 0; }
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase241Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase241ActorState();
		static_cast<FlowTestCase241Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(e.code() == error_code_connection_string_invalid);
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase241()
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase241Actor final : public Actor<Void>, public FastAllocated<FlowTestCase241Actor>, public FlowTestCase241ActorState<FlowTestCase241Actor> {
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase241Actor>::operator new;
	using FastAllocated<FlowTestCase241Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase241Actor(UnitTestParameters const& params) 
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase241ActorState<FlowTestCase241Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase241", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase241");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase241", reinterpret_cast<unsigned long>(this), -1);

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
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase241( UnitTestParameters const& params ) {
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase241Actor(params));
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase241, "/fdbclient/MonitorLeader/ConnectionString/hostname")

#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via tryResolveHostnamesImpl()
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class TryResolveHostnamesImplActor>
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class TryResolveHostnamesImplActorState {
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	TryResolveHostnamesImplActorState(ClusterConnectionString* const& self) 
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : self(self),
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allCoordinatorsSet()
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( const auto& coord : self->coords ) {
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				allCoordinatorsSet.insert(coord);
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::vector<Future<Void>> fs;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( auto& hostname : self->hostnames ) {
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				fs.push_back(map(hostname.resolve(), [&](Optional<NetworkAddress> const& addr) -> Void { if (addr.present()) { allCoordinatorsSet.insert(addr.get()); } return Void(); }));
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(fs);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<TryResolveHostnamesImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryResolveHostnamesImplActor*>(this)->actor_wait_state = 1;
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryResolveHostnamesImplActor, 0, Void >*>(static_cast<TryResolveHostnamesImplActor*>(this)));
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::vector<NetworkAddress> allCoordinators(allCoordinatorsSet.begin(), allCoordinatorsSet.end());
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::sort(allCoordinators.begin(), allCoordinators.end());
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<TryResolveHostnamesImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(allCoordinators); this->~TryResolveHostnamesImplActorState(); static_cast<TryResolveHostnamesImplActor*>(this)->destroy(); return 0; }
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<TryResolveHostnamesImplActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(allCoordinators);
		this->~TryResolveHostnamesImplActorState();
		static_cast<TryResolveHostnamesImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::vector<NetworkAddress> allCoordinators(allCoordinatorsSet.begin(), allCoordinatorsSet.end());
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		std::sort(allCoordinators.begin(), allCoordinators.end());
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<TryResolveHostnamesImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(allCoordinators); this->~TryResolveHostnamesImplActorState(); static_cast<TryResolveHostnamesImplActor*>(this)->destroy(); return 0; }
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClusterConnectionString* self;
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::set<NetworkAddress> allCoordinatorsSet;
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via tryResolveHostnamesImpl()
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class TryResolveHostnamesImplActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< TryResolveHostnamesImplActor, 0, Void >, public FastAllocated<TryResolveHostnamesImplActor>, public TryResolveHostnamesImplActorState<TryResolveHostnamesImplActor> {
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<TryResolveHostnamesImplActor>::operator new;
	using FastAllocated<TryResolveHostnamesImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryResolveHostnamesImplActor, 0, Void >;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	TryResolveHostnamesImplActor(ClusterConnectionString* const& self) 
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<std::vector<NetworkAddress>> tryResolveHostnamesImpl( ClusterConnectionString* const& self ) {
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new TryResolveHostnamesImplActor(self));
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

Future<std::vector<NetworkAddress>> ClusterConnectionString::tryResolveHostnames() {
	return tryResolveHostnamesImpl(this);
}

															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase294()
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase294Actor>
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase294ActorState {
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase294ActorState(UnitTestParameters const& params) 
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase294", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase294ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase294", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = "TestCluster:0@host.name:1234,host-name:5678";
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string hn = "host-name", port = "5678";
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			address = NetworkAddress::parse("1.0.0.0:5678");
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			INetworkConnections::net()->addMockTCPEndpoint(hn, port, { address });
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ClusterConnectionString cs(connectionString);
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = cs.tryResolveHostnames();
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<FlowTestCase294Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase294Actor*>(this)->actor_wait_state = 1;
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >*>(static_cast<FlowTestCase294Actor*>(this)));
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		this->~FlowTestCase294ActorState();
		static_cast<FlowTestCase294Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<NetworkAddress> const& allCoordinators,int loopDepth) 
	{
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		ASSERT(allCoordinators.size() == 1 && std::find(allCoordinators.begin(), allCoordinators.end(), address) != allCoordinators.end());
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase294Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase294ActorState(); static_cast<FlowTestCase294Actor*>(this)->destroy(); return 0; }
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase294Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase294ActorState();
		static_cast<FlowTestCase294Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<NetworkAddress> && allCoordinators,int loopDepth) 
	{
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		ASSERT(allCoordinators.size() == 1 && std::find(allCoordinators.begin(), allCoordinators.end(), address) != allCoordinators.end());
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<FlowTestCase294Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase294ActorState(); static_cast<FlowTestCase294Actor*>(this)->destroy(); return 0; }
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<FlowTestCase294Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase294ActorState();
		static_cast<FlowTestCase294Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase294Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase294Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase294Actor*>(this)->ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase294", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase294", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("flowTestCase294", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase294", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase294", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase294", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	NetworkAddress address;
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase294()
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase294Actor final : public Actor<Void>, public ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >, public FastAllocated<FlowTestCase294Actor>, public FlowTestCase294ActorState<FlowTestCase294Actor> {
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase294Actor>::operator new;
	using FastAllocated<FlowTestCase294Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >;
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase294Actor(UnitTestParameters const& params) 
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase294ActorState<FlowTestCase294Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase294", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase294");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase294", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase294Actor, 0, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase294( UnitTestParameters const& params ) {
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase294Actor(params));
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase294, "/fdbclient/MonitorLeader/PartialResolve")

#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase310()
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase310Actor>
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase310ActorState {
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase310ActorState(UnitTestParameters const& params) 
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase310", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase310ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase310", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				LeaderInfo in;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				LeaderInfo out;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				in.forward = deterministicRandom()->coinflip();
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				in.changeID = deterministicRandom()->randomUniqueID();
															#line 1233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					std::string rndString(deterministicRandom()->randomInt(10, 400), 'x');
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					for( auto& c : rndString ) {
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						c = deterministicRandom()->randomAlphaNumeric();
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					in.serializedInfo = rndString;
															#line 1245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ObjectWriter writer(IncludeVersion());
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				writer.serialize(in);
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				Standalone<StringRef> copy = writer.toStringRef();
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ArenaObjectReader reader(copy.arena(), copy, IncludeVersion());
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				reader.deserialize(out);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(in.forward == out.forward);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(in.changeID == out.changeID);
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(in.serializedInfo == out.serializedInfo);
															#line 1263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			LeaderInfo leaderInfo;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			leaderInfo.forward = deterministicRandom()->coinflip();
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			leaderInfo.changeID = deterministicRandom()->randomUniqueID();
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				std::string rndString(deterministicRandom()->randomInt(10, 400), 'x');
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				for( auto& c : rndString ) {
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					c = deterministicRandom()->randomAlphaNumeric();
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				leaderInfo.serializedInfo = rndString;
															#line 1283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ErrorOr<EnsureTable<Optional<LeaderInfo>>> objIn(leaderInfo);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ErrorOr<EnsureTable<Optional<LeaderInfo>>> objOut;
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			Standalone<StringRef> copy;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ObjectWriter writer(IncludeVersion());
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			writer.serialize(objIn);
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			copy = writer.toStringRef();
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ArenaObjectReader reader(copy.arena(), copy, IncludeVersion());
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			reader.deserialize(objOut);
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(!objOut.isError());
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(objOut.get().asUnderlyingType().present());
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			LeaderInfo outLeader = objOut.get().asUnderlyingType().get();
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(outLeader.changeID == leaderInfo.changeID);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(outLeader.forward == leaderInfo.forward);
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(outLeader.serializedInfo == leaderInfo.serializedInfo);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (!static_cast<FlowTestCase310Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase310ActorState(); static_cast<FlowTestCase310Actor*>(this)->destroy(); return 0; }
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			new (&static_cast<FlowTestCase310Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase310ActorState();
			static_cast<FlowTestCase310Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase310ActorState();
		static_cast<FlowTestCase310Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase310()
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase310Actor final : public Actor<Void>, public FastAllocated<FlowTestCase310Actor>, public FlowTestCase310ActorState<FlowTestCase310Actor> {
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase310Actor>::operator new;
	using FastAllocated<FlowTestCase310Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase310Actor(UnitTestParameters const& params) 
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase310ActorState<FlowTestCase310Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase310", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase310");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase310", reinterpret_cast<unsigned long>(this), -1);

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
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase310( UnitTestParameters const& params ) {
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase310Actor(params));
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase310, "/flow/FlatBuffers/LeaderInfo")

#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 1387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase360()
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class FlowTestCase360Actor>
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase360ActorState {
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase360ActorState(UnitTestParameters const& params) 
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : params(params)
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase360", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase360ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase360", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::string connectionString = "0xxdeadbeef:100100100@1.1.1.1:34534,5.1.5.3:23443";
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for(int i = 0;i < 10000;i++) {
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				std::string output("");
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				auto c = connectionString.begin();
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				for(;c != connectionString.end();) {
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					if (deterministicRandom()->random01() < 0.1)
															#line 1426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += deterministicRandom()->randomChoice(" \t\n\r"_sr);
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					if (deterministicRandom()->random01() < 0.5)
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					{
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += *c;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						++c;
															#line 1440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					if (deterministicRandom()->random01() < 0.1)
															#line 1444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += "#";
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						int charCount = deterministicRandom()->randomInt(0, 20);
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						for(int i = 0;i < charCount;i++) {
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
							output += deterministicRandom()->randomChoice("asdfzxcv123345:!@#$#$&()<\"\' \t"_sr);
															#line 1454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
						}
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
						output += deterministicRandom()->randomChoice("\n\r"_sr);
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
					}
				}
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterConnectionString cs(output);
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(connectionString == cs.toString());
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (!static_cast<FlowTestCase360Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase360ActorState(); static_cast<FlowTestCase360Actor*>(this)->destroy(); return 0; }
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			new (&static_cast<FlowTestCase360Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase360ActorState();
			static_cast<FlowTestCase360Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase360ActorState();
		static_cast<FlowTestCase360Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	UnitTestParameters params;
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase360()
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class FlowTestCase360Actor final : public Actor<Void>, public FastAllocated<FlowTestCase360Actor>, public FlowTestCase360ActorState<FlowTestCase360Actor> {
															#line 1498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase360Actor>::operator new;
	using FastAllocated<FlowTestCase360Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	FlowTestCase360Actor(UnitTestParameters const& params) 
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase360ActorState<FlowTestCase360Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase360", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase360");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase360", reinterpret_cast<unsigned long>(this), -1);

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
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
static Future<Void> flowTestCase360( UnitTestParameters const& params ) {
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new FlowTestCase360Actor(params));
															#line 1535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase360, "/fdbclient/MonitorLeader/parseConnectionString/fuzz")

#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

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

std::string ClientLeaderRegInterface::getAddressString() const {
	if (hostname.present()) {
		return hostname.get().toString();
	} else {
		return getLeader.getEndpoint().getPrimaryAddress().toString();
	}
}

// Nominee is the worker among all workers that are considered as leader by one coordinator
// This function contacts a coordinator coord to ask who is its nominee.
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorNominee()
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorNomineeActor>
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorNomineeActorState {
															#line 1655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorNomineeActorState(Key const& key,ClientLeaderRegInterface const& coord,AsyncTrigger* const& nomineeChange,Optional<LeaderInfo>* const& info) 
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : key(key),
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coord(coord),
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nomineeChange(nomineeChange),
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(info)
															#line 1668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		li = Optional<LeaderInfo>();
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = Future<Void>(Void());
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 1;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 0, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (coord.hostname.present())
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(li, retryGetReplyFromHostname(GetLeaderRequest(key, info->present() ? info->get().changeID : UID()), coord.hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 2;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 1, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store(li, retryBrokenPromise(coord.getLeader, GetLeaderRequest(key, info->present() ? info->get().changeID : UID()), TaskPriority::CoordinationReply));
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 3;
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 2, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (coord.hostname.present())
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(li, retryGetReplyFromHostname(GetLeaderRequest(key, info->present() ? info->get().changeID : UID()), coord.hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 2;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 1, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store(li, retryBrokenPromise(coord.getLeader, GetLeaderRequest(key, info->present() ? info->get().changeID : UID()), TaskPriority::CoordinationReply));
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 3;
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 2, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}

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
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_3 = Future<Void>(Void());
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 4;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 3, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 1871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state > 0) static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorNomineeActor*>(this)->ActorCallback< MonitorNomineeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
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
			a_body1loopBody1cont1when1(std::move(value), 0);
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
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void && _,int loopDepth) 
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
			a_body1loopBody1cont1when2(value, 0);
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
			a_body1loopBody1cont1when2(std::move(value), 0);
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
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetLeaderReply") .suppressFor(1.0) .detail("Coordinator", coord.getAddressString()) .detail("Nominee", li.present() ? li.get().changeID : UID()) .detail("ClusterKey", key.printable());
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (li != *info)
															#line 2032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			*info = li;
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nomineeChange->trigger();
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (li.present() && li.get().forward)
															#line 2040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_4 = Future<Void>(Never());
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont5when1(__when_expr_4.get(), loopDepth); };
				static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 5;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 4, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 2051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetLeaderReply") .suppressFor(1.0) .detail("Coordinator", coord.getAddressString()) .detail("Nominee", li.present() ? li.get().changeID : UID()) .detail("ClusterKey", key.printable());
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (li != *info)
															#line 2072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			*info = li;
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nomineeChange->trigger();
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (li.present() && li.get().forward)
															#line 2080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_4 = Future<Void>(Never());
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont5when1(__when_expr_4.get(), loopDepth); };
				static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 5;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< MonitorNomineeActor, 4, Void >*>(static_cast<MonitorNomineeActor*>(this)));
															#line 2091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(_), loopDepth);

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
			a_body1loopBody1cont2when1(value, 0);
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
			a_body1loopBody1cont2when1(std::move(value), 0);
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
	int a_body1loopBody1cont6(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<MonitorNomineeActor*>(this)->actor_wait_state > 0) static_cast<MonitorNomineeActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorNomineeActor*>(this)->ActorCallback< MonitorNomineeActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< MonitorNomineeActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< MonitorNomineeActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorNominee", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorNominee", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key key;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientLeaderRegInterface coord;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncTrigger* nomineeChange;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<LeaderInfo>* info;
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<LeaderInfo> li;
															#line 2266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorNominee()
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorNomineeActor final : public Actor<Void>, public ActorCallback< MonitorNomineeActor, 0, Void >, public ActorCallback< MonitorNomineeActor, 1, Void >, public ActorCallback< MonitorNomineeActor, 2, Void >, public ActorCallback< MonitorNomineeActor, 3, Void >, public ActorCallback< MonitorNomineeActor, 4, Void >, public FastAllocated<MonitorNomineeActor>, public MonitorNomineeActorState<MonitorNomineeActor> {
															#line 2271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
friend struct ActorCallback< MonitorNomineeActor, 4, Void >;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorNomineeActor(Key const& key,ClientLeaderRegInterface const& coord,AsyncTrigger* const& nomineeChange,Optional<LeaderInfo>* const& info) 
															#line 2286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		case 5: this->a_callback_error((ActorCallback< MonitorNomineeActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorNominee( Key const& key, ClientLeaderRegInterface const& coord, AsyncTrigger* const& nomineeChange, Optional<LeaderInfo>* const& info ) {
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorNomineeActor(key, coord, nomineeChange, info));
															#line 2318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

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
															#line 2371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorLeaderOneGeneration()
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorLeaderOneGenerationActor>
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderOneGenerationActorState {
															#line 2378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderOneGenerationActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo,MonitorLeaderInfo const& info) 
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   outSerializedLeaderInfo(outSerializedLeaderInfo),
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(info),
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinators(info.intermediateConnRecord),
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nomineeChange(),
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nominees(),
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allActors(),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   leader()
															#line 2399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nominees.resize(coordinators.clientLeaderServers.size());
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors = std::vector<Future<Void>>();
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors.reserve(coordinators.clientLeaderServers.size());
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for(int i = 0;i < coordinators.clientLeaderServers.size();i++) {
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				actors.push_back( monitorNominee(coordinators.clusterKey, coordinators.clientLeaderServers[i], &nomineeChange, &nominees[i]));
															#line 2422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			allActors = waitForAll(actors);
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 2428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		leader = getLeader(nominees);
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("MonitorLeaderChange") .detail("NewLeader", leader.present() ? leader.get().first.changeID : UID(1, 1));
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (leader.present())
															#line 2462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (leader.get().first.forward)
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("MonitorLeaderForwarding") .detail("NewConnStr", leader.get().first.serializedInfo.toString()) .detail("OldConnStr", info.intermediateConnRecord->getConnectionString().toString()) .trackLatest("MonitorLeaderForwarding");
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				info.intermediateConnRecord = connRecord->makeIntermediateRecord( ClusterConnectionString(leader.get().first.serializedInfo.toString()));
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!static_cast<MonitorLeaderOneGenerationActor*>(this)->SAV<MonitorLeaderInfo>::futures) { (void)(info); this->~MonitorLeaderOneGenerationActorState(); static_cast<MonitorLeaderOneGenerationActor*>(this)->destroy(); return 0; }
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				new (&static_cast<MonitorLeaderOneGenerationActor*>(this)->SAV< MonitorLeaderInfo >::value()) MonitorLeaderInfo(std::move(info)); // state_var_RVO
				this->~MonitorLeaderOneGenerationActorState();
				static_cast<MonitorLeaderOneGenerationActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (connRecord != info.intermediateConnRecord)
															#line 2482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!info.hasConnected)
															#line 2486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					TraceEvent(SevWarnAlways, "IncorrectClusterFileContentsAtConnection") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", connRecord->getConnectionString().toString()) .detail("CurrentConnectionString", info.intermediateConnRecord->getConnectionString().toString());
															#line 2490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_0 = connRecord->setAndPersistConnectionString(info.intermediateConnRecord->getConnectionString());
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state = 1;
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >*>(static_cast<MonitorLeaderOneGenerationActor*>(this)));
															#line 2501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_1 = nomineeChange.onTrigger() || allActors;
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state = 2;
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >*>(static_cast<MonitorLeaderOneGenerationActor*>(this)));
															#line 2527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.hasConnected = true;
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		connRecord->notifyConnected();
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		outSerializedLeaderInfo->set(leader.get().first.serializedInfo);
															#line 2540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord;
															#line 2549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord;
															#line 2558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

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
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorLeaderOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorLeaderOneGenerationActor*>(this)->ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorLeaderOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Value>> outSerializedLeaderInfo;
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientCoordinators coordinators;
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncTrigger nomineeChange;
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Optional<LeaderInfo>> nominees;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<Void> allActors;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<std::pair<LeaderInfo, bool>> leader;
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Future<Void>> actors;
															#line 2719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorLeaderOneGeneration()
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderOneGenerationActor final : public Actor<MonitorLeaderInfo>, public ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >, public ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >, public FastAllocated<MonitorLeaderOneGenerationActor>, public MonitorLeaderOneGenerationActorState<MonitorLeaderOneGenerationActor> {
															#line 2724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorLeaderOneGenerationActor>::operator new;
	using FastAllocated<MonitorLeaderOneGenerationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<MonitorLeaderInfo>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorLeaderOneGenerationActor, 0, Void >;
friend struct ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >;
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderOneGenerationActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo,MonitorLeaderInfo const& info) 
															#line 2736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		case 2: this->a_callback_error((ActorCallback< MonitorLeaderOneGenerationActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<MonitorLeaderInfo> monitorLeaderOneGeneration( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<Value>> const& outSerializedLeaderInfo, MonitorLeaderInfo const& info ) {
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<MonitorLeaderInfo>(new MonitorLeaderOneGenerationActor(connRecord, outSerializedLeaderInfo, info));
															#line 2765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 2770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorLeaderInternal()
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorLeaderInternalActor>
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderInternalActorState {
															#line 2777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInternalActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo) 
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   outSerializedLeaderInfo(outSerializedLeaderInfo),
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(connRecord)
															#line 2788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 2803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<MonitorLeaderInfo> __when_expr_0 = monitorLeaderOneGeneration(connRecord, outSerializedLeaderInfo, info);
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorLeaderInternalActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorLeaderInternalActor*>(this)->actor_wait_state = 1;
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >*>(static_cast<MonitorLeaderInternalActor*>(this)));
															#line 2840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(MonitorLeaderInfo const& _info,int loopDepth) 
	{
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 2849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(MonitorLeaderInfo && _info,int loopDepth) 
	{
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 2858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Value>> outSerializedLeaderInfo;
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 2932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorLeaderInternal()
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderInternalActor final : public Actor<Void>, public ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >, public FastAllocated<MonitorLeaderInternalActor>, public MonitorLeaderInternalActorState<MonitorLeaderInternalActor> {
															#line 2937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorLeaderInternalActor>::operator new;
	using FastAllocated<MonitorLeaderInternalActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorLeaderInternalActor, 0, MonitorLeaderInfo >;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInternalActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Value>> const& outSerializedLeaderInfo) 
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorLeaderInternal( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<Value>> const& outSerializedLeaderInfo ) {
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorLeaderInternalActor(connRecord, outSerializedLeaderInfo));
															#line 2976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 2981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via asyncDeserializeClusterInterface()
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class AsyncDeserializeClusterInterfaceActor>
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class AsyncDeserializeClusterInterfaceActorState {
															#line 2988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncDeserializeClusterInterfaceActorState(Reference<AsyncVar<Value>> const& serializedInfo,Reference<AsyncVar<Optional<ClusterInterface>>> const& outKnownLeader) 
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : serializedInfo(serializedInfo),
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   outKnownLeader(outKnownLeader),
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   knownLeader(new AsyncVar<Optional<ClusterControllerClientInterface>>{}),
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   deserializer(asyncDeserialize(serializedInfo, knownLeader))
															#line 3001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 3016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = deserializer;
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_1 = knownLeader->onChange();
															#line 3052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<AsyncDeserializeClusterInterfaceActor*>(this)->actor_wait_state = 1;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >*>(static_cast<AsyncDeserializeClusterInterfaceActor*>(this)));
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >*>(static_cast<AsyncDeserializeClusterInterfaceActor*>(this)));
															#line 3059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		UNSTOPPABLE_ASSERT(false);
															#line 3074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		UNSTOPPABLE_ASSERT(false);
															#line 3083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (knownLeader->get().present())
															#line 3092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(knownLeader->get().get().clientInterface);
															#line 3096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(Optional<ClusterInterface>{});
															#line 3102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (knownLeader->get().present())
															#line 3112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(knownLeader->get().get().clientInterface);
															#line 3116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			outKnownLeader->set(Optional<ClusterInterface>{});
															#line 3122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Value>> serializedInfo;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterInterface>>> outKnownLeader;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> knownLeader;
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<Void> deserializer;
															#line 3233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via asyncDeserializeClusterInterface()
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class AsyncDeserializeClusterInterfaceActor final : public Actor<Void>, public ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >, public ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >, public FastAllocated<AsyncDeserializeClusterInterfaceActor>, public AsyncDeserializeClusterInterfaceActorState<AsyncDeserializeClusterInterfaceActor> {
															#line 3238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<AsyncDeserializeClusterInterfaceActor>::operator new;
	using FastAllocated<AsyncDeserializeClusterInterfaceActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncDeserializeClusterInterfaceActor, 0, Void >;
friend struct ActorCallback< AsyncDeserializeClusterInterfaceActor, 1, Void >;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncDeserializeClusterInterfaceActor(Reference<AsyncVar<Value>> const& serializedInfo,Reference<AsyncVar<Optional<ClusterInterface>>> const& outKnownLeader) 
															#line 3250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> asyncDeserializeClusterInterface( Reference<AsyncVar<Value>> const& serializedInfo, Reference<AsyncVar<Optional<ClusterInterface>>> const& outKnownLeader ) {
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new AsyncDeserializeClusterInterfaceActor(serializedInfo, outKnownLeader));
															#line 3278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

namespace {

void tryInsertIntoSamples(OpenDatabaseRequest::Samples& samples,
                          const NetworkAddress& networkAddress,
                          const Key& traceLogGroup) {
	++samples.count;
	if (samples.samples.size() < static_cast<size_t>(CLIENT_KNOBS->CLIENT_EXAMPLE_AMOUNT)) {
		samples.samples.insert({ networkAddress, traceLogGroup });
	}
}

} // namespace

OpenDatabaseRequest ClientData::getRequest() {
	OpenDatabaseRequest req;

	for (auto& ci : clientStatusInfoMap) {
		const auto& networkAddress = ci.first;
		const auto& traceLogGroup = ci.second.traceLogGroup;

		for (auto& issue : ci.second.issues) {
			tryInsertIntoSamples(req.issues[issue], networkAddress, traceLogGroup);
		}

		if (!ci.second.versions.size()) {
			tryInsertIntoSamples(req.supportedVersions[ClientVersionRef()], networkAddress, traceLogGroup);
			continue;
		}

		++req.clientCount;
		StringRef maxProtocol;
		for (auto& it : ci.second.versions) {
			maxProtocol = std::max(maxProtocol, it.protocolVersion);
			tryInsertIntoSamples(req.supportedVersions[it], networkAddress, traceLogGroup);
		}
		tryInsertIntoSamples(req.maxProtocolSupported[maxProtocol], networkAddress, traceLogGroup);
	}

	return req;
}

															#line 3324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via getClientInfoFromLeader()
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class GetClientInfoFromLeaderActor>
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class GetClientInfoFromLeaderActorState {
															#line 3331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	GetClientInfoFromLeaderActorState(Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> const& knownLeader,ClientData* const& clientData) 
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : knownLeader(knownLeader),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientData(clientData)
															#line 3340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 3355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		lastRequestTime = now();
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req = clientData->getRequest();
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		;
															#line 3382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!(!knownLeader->get().present()))
															#line 3398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = knownLeader->onChange();
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state = 1;
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetClientInfoFromLeaderActor, 0, Void >*>(static_cast<GetClientInfoFromLeaderActor*>(this)));
															#line 3411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (now() - lastRequestTime > CLIENT_KNOBS->MAX_CLIENT_STATUS_AGE)
															#line 3515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			lastRequestTime = now();
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			req = clientData->getRequest();
															#line 3521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			resetReply(req);
															#line 3527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.knownClientInfoID = clientData->clientInfo->get().read().id;
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<ClientDBInfo> __when_expr_1 = brokenPromiseToNever(knownLeader->get().get().clientInterface.openDatabase.getReply(req));
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_2 = knownLeader->onChange();
															#line 3539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when2(__when_expr_2.get(), loopDepth); };
		static_cast<GetClientInfoFromLeaderActor*>(this)->actor_wait_state = 2;
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >*>(static_cast<GetClientInfoFromLeaderActor*>(this)));
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetClientInfoFromLeaderActor, 2, Void >*>(static_cast<GetClientInfoFromLeaderActor*>(this)));
															#line 3546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetClientInfoFromLeaderGotClientInfo", knownLeader->get().get().clientInterface.id()) .detail("CommitProxy0", ni.commitProxies.size() ? ni.commitProxies[0].address().toString() : "") .detail("GrvProxy0", ni.grvProxies.size() ? ni.grvProxies[0].address().toString() : "") .detail("ClientID", ni.id);
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientData->clientInfo->set(CachedSerialization<ClientDBInfo>(ni));
															#line 3563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(ClientDBInfo && ni,int loopDepth) 
	{
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("GetClientInfoFromLeaderGotClientInfo", knownLeader->get().get().clientInterface.id()) .detail("CommitProxy0", ni.commitProxies.size() ? ni.commitProxies[0].address().toString() : "") .detail("GrvProxy0", ni.grvProxies.size() ? ni.grvProxies[0].address().toString() : "") .detail("ClientID", ni.id);
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientData->clientInfo->set(CachedSerialization<ClientDBInfo>(ni));
															#line 3574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> knownLeader;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientData* clientData;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	double lastRequestTime;
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	OpenDatabaseRequest req;
															#line 3696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via getClientInfoFromLeader()
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class GetClientInfoFromLeaderActor final : public Actor<Void>, public ActorCallback< GetClientInfoFromLeaderActor, 0, Void >, public ActorCallback< GetClientInfoFromLeaderActor, 1, ClientDBInfo >, public ActorCallback< GetClientInfoFromLeaderActor, 2, Void >, public FastAllocated<GetClientInfoFromLeaderActor>, public GetClientInfoFromLeaderActorState<GetClientInfoFromLeaderActor> {
															#line 3701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	GetClientInfoFromLeaderActor(Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> const& knownLeader,ClientData* const& clientData) 
															#line 3714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> getClientInfoFromLeader( Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> const& knownLeader, ClientData* const& clientData ) {
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new GetClientInfoFromLeaderActor(knownLeader, clientData));
															#line 3743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 3748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorLeaderAndGetClientInfo()
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorLeaderAndGetClientInfoActor>
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderAndGetClientInfoActorState {
															#line 3755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderAndGetClientInfoActorState(Key const& clusterKey,std::vector<Hostname> const& hostnames,std::vector<NetworkAddress> const& coordinators,ClientData* const& clientData,Reference<AsyncVar<Optional<LeaderInfo>>> const& leaderInfo) 
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : clusterKey(clusterKey),
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   hostnames(hostnames),
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinators(coordinators),
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientData(clientData),
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   leaderInfo(leaderInfo),
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientLeaderServers(),
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nomineeChange(),
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   nominees(),
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allActors(),
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   knownLeader(new AsyncVar<Optional<ClusterControllerClientInterface>>{})
															#line 3780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			clientLeaderServers.reserve(hostnames.size() + coordinators.size());
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( auto h : hostnames ) {
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(h));
															#line 3799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( auto s : coordinators ) {
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(s));
															#line 3805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			nominees.resize(clientLeaderServers.size());
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			std::vector<Future<Void>> actors;
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors.reserve(clientLeaderServers.size());
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for(int i = 0;i < clientLeaderServers.size();i++) {
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				actors.push_back(monitorNominee(clusterKey, clientLeaderServers[i], &nomineeChange, &nominees[i]));
															#line 3817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			actors.push_back(getClientInfoFromLeader(knownLeader, clientData));
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			allActors = waitForAll(actors);
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 3825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		Optional<std::pair<LeaderInfo, bool>> leader = getLeader(nominees);
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		TraceEvent("MonitorLeaderAndGetClientInfoLeaderChange") .detail("NewLeader", leader.present() ? leader.get().first.changeID : UID(1, 1)) .detail("Key", clusterKey.printable());
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (leader.present())
															#line 3859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (leader.get().first.forward)
															#line 3863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClientDBInfo outInfo;
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				outInfo.id = deterministicRandom()->randomUniqueID();
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				outInfo.forward = leader.get().first.serializedInfo;
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientData->clientInfo->set(CachedSerialization<ClientDBInfo>(outInfo));
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				leaderInfo->set(leader.get().first);
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("MonitorLeaderAndGetClientInfoForwarding") .detail("NewConnStr", leader.get().first.serializedInfo.toString());
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MonitorLeaderAndGetClientInfoActorState(); static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->destroy(); return 0; }
															#line 3879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				new (&static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~MonitorLeaderAndGetClientInfoActorState();
				static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (leader.get().first.serializedInfo.size())
															#line 3887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ObjectReader reader(leader.get().first.serializedInfo.begin(), IncludeVersion());
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ClusterControllerClientInterface res;
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				reader.deserialize(res);
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				knownLeader->set(res);
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				leaderInfo->set(leader.get().first);
															#line 3899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
		}
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = nomineeChange.onTrigger() || allActors;
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorLeaderAndGetClientInfoActor*>(this)->actor_wait_state = 1;
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >*>(static_cast<MonitorLeaderAndGetClientInfoActor*>(this)));
															#line 3911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key clusterKey;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Hostname> hostnames;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<NetworkAddress> coordinators;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientData* clientData;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<LeaderInfo>>> leaderInfo;
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<ClientLeaderRegInterface> clientLeaderServers;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	AsyncTrigger nomineeChange;
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<Optional<LeaderInfo>> nominees;
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<Void> allActors;
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClusterControllerClientInterface>>> knownLeader;
															#line 4011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorLeaderAndGetClientInfo()
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorLeaderAndGetClientInfoActor final : public Actor<Void>, public ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >, public FastAllocated<MonitorLeaderAndGetClientInfoActor>, public MonitorLeaderAndGetClientInfoActorState<MonitorLeaderAndGetClientInfoActor> {
															#line 4016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorLeaderAndGetClientInfoActor>::operator new;
	using FastAllocated<MonitorLeaderAndGetClientInfoActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorLeaderAndGetClientInfoActor, 0, Void >;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderAndGetClientInfoActor(Key const& clusterKey,std::vector<Hostname> const& hostnames,std::vector<NetworkAddress> const& coordinators,ClientData* const& clientData,Reference<AsyncVar<Optional<LeaderInfo>>> const& leaderInfo) 
															#line 4027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorLeaderAndGetClientInfo( Key const& clusterKey, std::vector<Hostname> const& hostnames, std::vector<NetworkAddress> const& coordinators, ClientData* const& clientData, Reference<AsyncVar<Optional<LeaderInfo>>> const& leaderInfo ) {
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorLeaderAndGetClientInfoActor(clusterKey, hostnames, coordinators, clientData, leaderInfo));
															#line 4055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

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

															#line 4100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorProxiesOneGeneration()
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorProxiesOneGenerationActor>
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesOneGenerationActorState {
															#line 4107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesOneGenerationActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,MonitorLeaderInfo const& info,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientInfo(clientInfo),
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinator(coordinator),
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(info),
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   supportedVersions(supportedVersions),
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   traceLogGroup(traceLogGroup),
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   internal(internal),
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   cs(info.intermediateConnRecord->getConnectionString()),
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinatorsSize(cs.hostnames.size() + cs.coords.size()),
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   index(0),
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   successIndex(0),
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   incorrectTime(),
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastCommitProxyUIDs(),
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastCommitProxies(),
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastGrvProxyUIDs(),
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   lastGrvProxies(),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientLeaderServers(),
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   allConnectionsFailed(false)
															#line 4148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			clientLeaderServers.reserve(coordinatorsSize);
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( const auto& h : cs.hostnames ) {
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(h));
															#line 4167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			for( const auto& c : cs.coords ) {
															#line 865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				clientLeaderServers.push_back(ClientLeaderRegInterface(c));
															#line 4173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			}
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			ASSERT(clientLeaderServers.size() > 0);
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			deterministicRandom()->randomShuffle(clientLeaderServers);
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 4181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientLeaderServer = clientLeaderServers[index];
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req = OpenDatabaseCoordRequest();
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.clusterKey = cs.clusterKey();
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.hostnames = cs.hostnames;
															#line 877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.coordinators = cs.coords;
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.knownClientInfoID = clientInfo->get().id;
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.supportedVersions = supportedVersions->get();
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.traceLogGroup = traceLogGroup;
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		req.internal = internal;
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		storedConnectionString = ClusterConnectionString();
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (connRecord)
															#line 4231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			StrictFuture<bool> __when_expr_0 = connRecord->upToDate(storedConnectionString);
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 1;
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			incorrectTime = Optional<double>();
															#line 4249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		repFuture = Future<ErrorOr<CachedSerialization<ClientDBInfo>>>();
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (clientLeaderServer.hostname.present())
															#line 4261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			repFuture = tryGetReplyFromHostname(req, clientLeaderServer.hostname.get(), WLTOKEN_CLIENTLEADERREG_OPENDATABASE, TaskPriority::CoordinationReply);
															#line 4265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
		else
		{
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			repFuture = clientLeaderServer.openDatabase.tryGetReply(req, TaskPriority::CoordinationReply);
															#line 4271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		}
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		coordinator->setUnconditional(clientLeaderServer);
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<ErrorOr<CachedSerialization<ClientDBInfo>>> __when_expr_2 = repFuture;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 3;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& upToDate,int loopDepth) 
	{
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (upToDate)
															#line 4293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			incorrectTime = Optional<double>();
															#line 4297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}
		else
		{
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (allConnectionsFailed && storedConnectionString.getNumberOfCoordinators() > 0)
															#line 4304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("UpdatingConnectionStringFromFile") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", storedConnectionString.toString()) .detail("CurrentConnectionString", connRecord->getConnectionString().toString());
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_1 = connRecord->setAndPersistConnectionString(storedConnectionString);
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
				static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 2;
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				req.issues.push_back_deep(req.issues.arena(), "incorrect_cluster_file_contents"_sr);
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				std::string connectionString = connRecord->getConnectionString().toString();
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!incorrectTime.present())
															#line 4328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					incorrectTime = now();
															#line 4332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent(now() - incorrectTime.get() > 300 ? SevWarnAlways : SevWarn, "IncorrectClusterFileContents") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", storedConnectionString.toString()) .detail("CurrentConnectionString", connectionString);
															#line 4336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = a_body1loopBody1cont5(loopDepth);
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && upToDate,int loopDepth) 
	{
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (upToDate)
															#line 4347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			incorrectTime = Optional<double>();
															#line 4351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}
		else
		{
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (allConnectionsFailed && storedConnectionString.getNumberOfCoordinators() > 0)
															#line 4358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("UpdatingConnectionStringFromFile") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", storedConnectionString.toString()) .detail("CurrentConnectionString", connRecord->getConnectionString().toString());
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_1 = connRecord->setAndPersistConnectionString(storedConnectionString);
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
				static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 2;
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				req.issues.push_back_deep(req.issues.arena(), "incorrect_cluster_file_contents"_sr);
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				std::string connectionString = connRecord->getConnectionString().toString();
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!incorrectTime.present())
															#line 4382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					incorrectTime = now();
															#line 4386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent(now() - incorrectTime.get() > 300 ? SevWarnAlways : SevWarn, "IncorrectClusterFileContents") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", storedConnectionString.toString()) .detail("CurrentConnectionString", connectionString);
															#line 4390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = a_body1loopBody1cont5(loopDepth);
			}
		}

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
	int a_body1loopBody1cont3(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord;
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV<MonitorLeaderInfo>::futures) { (void)(info); this->~MonitorProxiesOneGenerationActorState(); static_cast<MonitorProxiesOneGenerationActor*>(this)->destroy(); return 0; }
															#line 4478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV< MonitorLeaderInfo >::value()) MonitorLeaderInfo(std::move(info)); // state_var_RVO
		this->~MonitorProxiesOneGenerationActorState();
		static_cast<MonitorProxiesOneGenerationActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord;
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (!static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV<MonitorLeaderInfo>::futures) { (void)(info); this->~MonitorProxiesOneGenerationActorState(); static_cast<MonitorProxiesOneGenerationActor*>(this)->destroy(); return 0; }
															#line 4492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		new (&static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV< MonitorLeaderInfo >::value()) MonitorLeaderInfo(std::move(info)); // state_var_RVO
		this->~MonitorProxiesOneGenerationActorState();
		static_cast<MonitorProxiesOneGenerationActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >*,Error err) 
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
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (rep.present())
															#line 4567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		{
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (rep.get().read().forward.present())
															#line 4571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				TraceEvent("MonitorProxiesForwarding") .detail("NewConnStr", rep.get().read().forward.get().toString()) .detail("OldConnStr", info.intermediateConnRecord->getConnectionString().toString());
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				info.intermediateConnRecord = connRecord->makeIntermediateRecord( ClusterConnectionString(rep.get().read().forward.get().toString()));
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				ASSERT(info.intermediateConnRecord->getConnectionString().getNumberOfCoordinators() > 0);
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV<MonitorLeaderInfo>::futures) { (void)(info); this->~MonitorProxiesOneGenerationActorState(); static_cast<MonitorProxiesOneGenerationActor*>(this)->destroy(); return 0; }
															#line 4581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				new (&static_cast<MonitorProxiesOneGenerationActor*>(this)->SAV< MonitorLeaderInfo >::value()) MonitorLeaderInfo(std::move(info)); // state_var_RVO
				this->~MonitorProxiesOneGenerationActorState();
				static_cast<MonitorProxiesOneGenerationActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (connRecord != info.intermediateConnRecord)
															#line 4589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (!info.hasConnected)
															#line 4593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				{
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
					TraceEvent(SevWarnAlways, "IncorrectClusterFileContentsAtConnection") .detail("ClusterFile", connRecord->toString()) .detail("StoredConnectionString", connRecord->getConnectionString().toString()) .detail("CurrentConnectionString", info.intermediateConnRecord->getConnectionString().toString());
															#line 4597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				}
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_3 = connRecord->setAndPersistConnectionString(info.intermediateConnRecord->getConnectionString());
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont11when1(__when_expr_3.get(), loopDepth); };
				static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 4;
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont11cont2(loopDepth);
			}
		}
		else
		{
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			CODE_PROBE(rep.getError().code() == error_code_failed_to_progress, "Coordinator cannot talk to cluster controller");
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			TraceEvent("MonitorProxiesConnectFailed") .detail("Error", rep.getError().name()) .detail("Coordinator", clientLeaderServer.getAddressString());
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			index = (index + 1) % coordinatorsSize;
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			if (index == successIndex)
															#line 4626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
			{
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				allConnectionsFailed = true;
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				StrictFuture<Void> __when_expr_4 = delay(CLIENT_KNOBS->COORDINATOR_RECONNECTION_DELAY);
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont11when2(__when_expr_4.get(), loopDepth); };
				static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 5;
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >*>(static_cast<MonitorProxiesOneGenerationActor*>(this)));
															#line 4639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont11cont8(loopDepth);
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<CachedSerialization<ClientDBInfo>> const& __rep,int loopDepth) 
	{
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		rep = __rep;
															#line 4654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<CachedSerialization<ClientDBInfo>> && __rep,int loopDepth) 
	{
		rep = std::move(__rep);
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >*,ErrorOr<CachedSerialization<ClientDBInfo>> const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >*,ErrorOr<CachedSerialization<ClientDBInfo>> && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >*,Error err) 
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
	int a_body1loopBody1cont11cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont2(int loopDepth) 
	{
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.hasConnected = true;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		connRecord->notifyConnected();
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		auto& ni = rep.get().mutate();
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		shrinkProxyList(ni, lastCommitProxyUIDs, lastCommitProxies, lastGrvProxyUIDs, lastGrvProxies);
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		clientInfo->setUnconditional(ni);
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		successIndex = index;
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		allConnectionsFailed = false;
															#line 4739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont4(Void const& _,int loopDepth) 
	{
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord;
															#line 4748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont4(Void && _,int loopDepth) 
	{
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord;
															#line 4757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont11when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont11when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont11cont8(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state > 0) static_cast<MonitorProxiesOneGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorProxiesOneGenerationActor*>(this)->ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont11when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont11when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitorProxiesOneGeneration", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<ClientDBInfo>> clientInfo;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> coordinator;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> supportedVersions;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key traceLogGroup;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	IsInternal internal;
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClusterConnectionString cs;
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	int coordinatorsSize;
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	int index;
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	int successIndex;
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Optional<double> incorrectTime;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<UID> lastCommitProxyUIDs;
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<CommitProxyInterface> lastCommitProxies;
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<UID> lastGrvProxyUIDs;
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<GrvProxyInterface> lastGrvProxies;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	std::vector<ClientLeaderRegInterface> clientLeaderServers;
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	bool allConnectionsFailed;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClientLeaderRegInterface clientLeaderServer;
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	OpenDatabaseCoordRequest req;
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ClusterConnectionString storedConnectionString;
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Future<ErrorOr<CachedSerialization<ClientDBInfo>>> repFuture;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	ErrorOr<CachedSerialization<ClientDBInfo>> rep;
															#line 4952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorProxiesOneGeneration()
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesOneGenerationActor final : public Actor<MonitorLeaderInfo>, public ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >, public ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >, public ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >, public ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >, public ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >, public FastAllocated<MonitorProxiesOneGenerationActor>, public MonitorProxiesOneGenerationActorState<MonitorProxiesOneGenerationActor> {
															#line 4957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorProxiesOneGenerationActor>::operator new;
	using FastAllocated<MonitorProxiesOneGenerationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<MonitorLeaderInfo>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 0, bool >;
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >;
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >;
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >;
friend struct ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >;
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesOneGenerationActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,MonitorLeaderInfo const& info,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 4972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
		case 2: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 2, ErrorOr<CachedSerialization<ClientDBInfo>> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< MonitorProxiesOneGenerationActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<MonitorLeaderInfo> monitorProxiesOneGeneration( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<ClientDBInfo>> const& clientInfo, Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator, MonitorLeaderInfo const& info, Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions, Key const& traceLogGroup, IsInternal const& internal ) {
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<MonitorLeaderInfo>(new MonitorProxiesOneGenerationActor(connRecord, clientInfo, coordinator, info, supportedVersions, traceLogGroup, internal));
															#line 5004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"

															#line 5009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
namespace {
// This generated class is to be used only via monitorProxies()
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
template <class MonitorProxiesActor>
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesActorState {
															#line 5016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesActorState(Reference<AsyncVar<Reference<IClusterConnectionRecord>>> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		 : connRecord(connRecord),
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   clientInfo(clientInfo),
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   coordinator(coordinator),
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   supportedVersions(supportedVersions),
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   traceLogGroup(traceLogGroup),
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   internal(internal),
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		   info(connRecord->get())
															#line 5035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
			;
															#line 5050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		ASSERT(connRecord->get().isValid());
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<MonitorLeaderInfo> __when_expr_0 = monitorProxiesOneGeneration( connRecord->get(), clientInfo, coordinator, info, supportedVersions, traceLogGroup, internal);
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		if (static_cast<MonitorProxiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		StrictFuture<Void> __when_expr_1 = connRecord->onChange();
															#line 5088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<MonitorProxiesActor*>(this)->actor_wait_state = 1;
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >*>(static_cast<MonitorProxiesActor*>(this)));
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MonitorProxiesActor, 1, Void >*>(static_cast<MonitorProxiesActor*>(this)));
															#line 5095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 5110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(MonitorLeaderInfo && _info,int loopDepth) 
	{
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info = _info;
															#line 5119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.hasConnected = false;
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord->get();
															#line 5130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.hasConnected = false;
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
		info.intermediateConnRecord = connRecord->get();
															#line 5141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Reference<IClusterConnectionRecord>>> connRecord;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<ClientDBInfo>> clientInfo;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> coordinator;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> supportedVersions;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	Key traceLogGroup;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	IsInternal internal;
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorLeaderInfo info;
															#line 5257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
};
// This generated class is to be used only via monitorProxies()
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
class MonitorProxiesActor final : public Actor<Void>, public ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >, public ActorCallback< MonitorProxiesActor, 1, Void >, public FastAllocated<MonitorProxiesActor>, public MonitorProxiesActorState<MonitorProxiesActor> {
															#line 5262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
public:
	using FastAllocated<MonitorProxiesActor>::operator new;
	using FastAllocated<MonitorProxiesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorProxiesActor, 0, MonitorLeaderInfo >;
friend struct ActorCallback< MonitorProxiesActor, 1, Void >;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	MonitorProxiesActor(Reference<AsyncVar<Reference<IClusterConnectionRecord>>> const& connRecord,Reference<AsyncVar<ClientDBInfo>> const& clientInfo,Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator,Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions,Key const& traceLogGroup,IsInternal const& internal) 
															#line 5274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
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
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
[[nodiscard]] Future<Void> monitorProxies( Reference<AsyncVar<Reference<IClusterConnectionRecord>>> const& connRecord, Reference<AsyncVar<ClientDBInfo>> const& clientInfo, Reference<AsyncVar<Optional<ClientLeaderRegInterface>>> const& coordinator, Reference<ReferencedObject<Standalone<VectorRef<ClientVersionRef>>>> const& supportedVersions, Key const& traceLogGroup, IsInternal const& internal ) {
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
	return Future<Void>(new MonitorProxiesActor(connRecord, clientInfo, coordinator, supportedVersions, traceLogGroup, internal));
															#line 5302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.g.cpp"
}

#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/MonitorLeader.actor.cpp"
