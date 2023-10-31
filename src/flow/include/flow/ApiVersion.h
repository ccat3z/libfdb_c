/*
 * ApiVersion.h
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

#ifndef FLOW_CODE_API_VERSION_H
#define FLOW_CODE_API_VERSION_H

#pragma once
#include "flow/Trace.h"
#include <cstdint>

constexpr int noBackwardsCompatibility = 13;

// The first check second expression version doesn't need to change because it's just for earlier API versions.
#define API_VERSION_FEATURE(v, x)                                                                                      \
	static_assert(v <= 730, "Feature API version too large");                                   \
	struct x {                                                                                                         \
		static constexpr uint64_t apiVersion = v;                                                                      \
	};                                                                                                                 \
	constexpr bool has##x() const { return this->version() >= x ::apiVersion; }                                        \
	static constexpr ApiVersion with##x() { return ApiVersion(x ::apiVersion); }

class ApiVersion {
	int _version;

public:
	// Statics.
	constexpr static int LATEST_VERSION = 730;

	constexpr explicit ApiVersion(int version) : _version(version) {}
	constexpr ApiVersion() : _version(0) {}

	constexpr bool isValid() const {
		return version() > noBackwardsCompatibility && version() <= LATEST_VERSION;
	}

	constexpr int version() const { return _version; }

	// comparison operators
	constexpr bool operator==(const ApiVersion other) const { return version() == other.version(); }
	constexpr bool operator!=(const ApiVersion other) const { return version() != other.version(); }
	constexpr bool operator<=(const ApiVersion other) const { return version() <= other.version(); }
	constexpr bool operator>=(const ApiVersion other) const { return version() >= other.version(); }
	constexpr bool operator<(const ApiVersion other) const { return version() < other.version(); }
	constexpr bool operator>(const ApiVersion other) const { return version() > other.version(); }

public: // introduced features
    API_VERSION_FEATURE(300, SnapshotRYW);
    API_VERSION_FEATURE(610, InlineUpdateDatabase);
    API_VERSION_FEATURE(610, PersistentOptions);
    API_VERSION_FEATURE(630, TraceFileIdentifier);
    API_VERSION_FEATURE(710, ClusterSharedStateMap);
    API_VERSION_FEATURE(720, BlobRangeApi);
    API_VERSION_FEATURE(720, CreateDBFromConnString);
    API_VERSION_FEATURE(720, FutureGetBool);
    API_VERSION_FEATURE(720, FutureProtocolVersionApi);
    API_VERSION_FEATURE(720, TenantBlobRangeApi);
    API_VERSION_FEATURE(720, ClientTmpDir);
    API_VERSION_FEATURE(720, DisableClientBypass)
    API_VERSION_FEATURE(720, GrvCache);
    API_VERSION_FEATURE(720, ClientProfilingDeprecated);
    API_VERSION_FEATURE(720, TenantApiReleased);
    API_VERSION_FEATURE(730, GetTotalCost);
    API_VERSION_FEATURE(730, FailOnExternalClientErrors);
    API_VERSION_FEATURE(730, GetTagThrottledDuration);
    API_VERSION_FEATURE(730, FutureGetDouble);
    API_VERSION_FEATURE(730, GetClientStatus);
    API_VERSION_FEATURE(730, InitializeTraceOnSetup);
    API_VERSION_FEATURE(730, TenantGetId);
};

#endif // FLOW_CODE_API_VERSION_H
