#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
#include "fdbrpc/Base64Encode.h"
#include "fdbrpc/Base64Decode.h"
#include "fdbrpc/FlowTransport.h"
#include "fdbrpc/TokenCache.h"
#include "fdbrpc/TokenSign.h"
#include "fdbrpc/TenantInfo.h"
#include "flow/MkCert.h"
#include "flow/ScopeExit.h"
#include "flow/UnitTest.h"
#include "flow/network.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <fmt/format.h>
#include <list>
#include <deque>

#include "flow/actorcompiler.h" // has to be last include

using authz::TenantId;

template <class Key, class Value>
class LRUCache {
public:
	using key_type = Key;
	using list_type = std::list<key_type>;
	using mapped_type = Value;
	using map_type = boost::unordered_map<key_type, std::pair<mapped_type, typename list_type::iterator>>;
	using size_type = unsigned;

	explicit LRUCache(size_type capacity) : _capacity(capacity) { _map.reserve(capacity); }

	size_type size() const { return _map.size(); }
	size_type capacity() const { return _capacity; }
	bool empty() const { return _map.empty(); }

	Optional<mapped_type*> get(key_type const& key) {
		auto i = _map.find(key);
		if (i == _map.end()) {
			return Optional<mapped_type*>();
		}
		auto j = i->second.second;
		if (j != _list.begin()) {
			_list.erase(j);
			_list.push_front(i->first);
			i->second.second = _list.begin();
		}
		return &i->second.first;
	}

	template <class K, class V>
	mapped_type* insert(K&& key, V&& value) {
		auto iter = _map.find(key);
		if (iter != _map.end()) {
			return &iter->second.first;
		}
		if (size() == capacity()) {
			auto i = --_list.end();
			_map.erase(*i);
			_list.erase(i);
		}
		_list.push_front(std::forward<K>(key));
		std::tie(iter, std::ignore) =
		    _map.insert(std::make_pair(*_list.begin(), std::make_pair(std::forward<V>(value), _list.begin())));
		return &iter->second.first;
	}

private:
	const size_type _capacity;
	map_type _map;
	list_type _list;
};

															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase79()
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
template <class FlowTestCase79Actor>
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
class FlowTestCase79ActorState {
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
public:
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	FlowTestCase79ActorState(UnitTestParameters const& params) 
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		 : params(params)
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase79", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase79ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase79", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto& rng = *deterministicRandom();
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				LRUCache<int, StringRef> cache(rng.randomInt(2, 10));
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				for(int i = 0;i < 200;++i) {
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cache.insert(i, "val"_sr);
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					if (i >= cache.capacity())
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
					{
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						for(auto j = 0;j <= i - cache.capacity();j++) {
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							ASSERT(!cache.get(j).present());
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
						}
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						for(auto j = i - cache.capacity() + 1;j <= i;j++) {
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							ASSERT(cache.get(j).present());
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
						}
					}
				}
			}
			{
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				LRUCache<int, StringRef> cache(1000);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				for(auto i = 0;i < 1000;++i) {
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cache.insert(i, "value"_sr);
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
				}
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				cache.insert(1000, "value"_sr);
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				ASSERT(!cache.get(0).present());
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			}
			{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				LRUCache<StringRef, Standalone<StringRef>> cache(10);
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				std::deque<std::string> cachedStrings;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				std::deque<std::string> evictedStrings;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				for(int i = 0;i < 10;++i) {
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					auto str = rng.randomAlphaNumeric(rng.randomInt(100, 1024));
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					Standalone<StringRef> sref(str);
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cache.insert(sref, sref);
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cachedStrings.push_back(str);
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
				}
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				for(int i = 0;i < 10;++i) {
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					Standalone<StringRef> existingStr(cachedStrings.back());
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					auto cachedStr = cache.get(existingStr);
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					ASSERT(cachedStr.present());
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					ASSERT(*cachedStr.get() == existingStr);
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					if (!evictedStrings.empty())
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
					{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						Standalone<StringRef> nonexisting(evictedStrings.at(rng.randomInt(0, evictedStrings.size())));
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						ASSERT(!cache.get(nonexisting).present());
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
					}
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					auto str = rng.randomAlphaNumeric(rng.randomInt(100, 1024));
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					Standalone<StringRef> sref(str);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					evictedStrings.push_back(cachedStrings.front());
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cachedStrings.pop_front();
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cachedStrings.push_back(str);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					cache.insert(sref, sref);
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
				}
			}
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (!static_cast<FlowTestCase79Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase79ActorState(); static_cast<FlowTestCase79Actor*>(this)->destroy(); return 0; }
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			new (&static_cast<FlowTestCase79Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase79ActorState();
			static_cast<FlowTestCase79Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase79ActorState();
		static_cast<FlowTestCase79Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	UnitTestParameters params;
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase79()
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
class FlowTestCase79Actor final : public Actor<Void>, public FastAllocated<FlowTestCase79Actor>, public FlowTestCase79ActorState<FlowTestCase79Actor> {
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase79Actor>::operator new;
	using FastAllocated<FlowTestCase79Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	FlowTestCase79Actor(UnitTestParameters const& params) 
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase79ActorState<FlowTestCase79Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase79", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase79");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase79", reinterpret_cast<unsigned long>(this), -1);

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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
static Future<Void> flowTestCase79( UnitTestParameters const& params ) {
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	return Future<Void>(new FlowTestCase79Actor(params));
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase79, "/fdbrpc/authz/LRUCache")

#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"

struct CacheEntry {
	Arena arena;
	VectorRef<TenantId> tenants;
	Optional<StringRef> tokenId;
	double expirationTime = 0.0;
};

struct AuditEntry {
	NetworkAddress address;
	TenantId tenantId;
	Optional<Standalone<StringRef>> tokenId;
	bool operator==(const AuditEntry& other) const noexcept = default;
	explicit AuditEntry(NetworkAddress const& address, TenantId tenantId, CacheEntry const& cacheEntry)
	  : address(address), tenantId(tenantId),
	    tokenId(cacheEntry.tokenId.present() ? Standalone<StringRef>(cacheEntry.tokenId.get(), cacheEntry.arena)
	                                         : Optional<Standalone<StringRef>>()) {}
};

std::size_t hash_value(AuditEntry const& value) {
	std::size_t seed = 0;
	boost::hash_combine(seed, value.address);
	boost::hash_combine(seed, value.tenantId);
	if (value.tokenId.present()) {
		boost::hash_combine(seed, value.tokenId.get());
	}
	return seed;
}

struct TokenCacheImpl {
	TokenCacheImpl();
	LRUCache<StringRef, CacheEntry> cache;
	boost::unordered_set<AuditEntry> usedTokens;
	double lastResetTime;

	bool validate(TenantId tenantId, StringRef token);
	bool validateAndAdd(double currentTime, StringRef token, NetworkAddress const& peer);
	void logTokenUsage(double currentTime, AuditEntry&& entry);
};

TokenCacheImpl::TokenCacheImpl() : cache(FLOW_KNOBS->TOKEN_CACHE_SIZE), usedTokens(), lastResetTime(0) {}

TokenCache::TokenCache() : impl(new TokenCacheImpl()) {}
TokenCache::~TokenCache() {
	delete impl;
}

void TokenCache::createInstance() {
	g_network->setGlobal(INetwork::enTokenCache, new TokenCache());
}

TokenCache& TokenCache::instance() {
	return *reinterpret_cast<TokenCache*>(g_network->global(INetwork::enTokenCache));
}

bool TokenCache::validate(TenantId tenantId, StringRef token) {
	return impl->validate(tenantId, token);
}

#define TRACE_INVALID_PARSED_TOKEN(reason, token)                                                                      \
	TraceEvent(SevWarn, "InvalidToken"_audit)                                                                          \
	    .detail("From", peer)                                                                                          \
	    .detail("Reason", reason)                                                                                      \
	    .detail("CurrentTime", currentTime)                                                                            \
	    .detail("Token", toStringRef(arena, token).toStringView())

bool TokenCacheImpl::validateAndAdd(double currentTime, StringRef token, NetworkAddress const& peer) {
	Arena arena;
	authz::jwt::TokenRef t;
	StringRef signInput;
	Optional<StringRef> err;
	bool verifyOutcome;
	if ((err = authz::jwt::parseToken(arena, token, t, signInput)).present()) {
		CODE_PROBE(true, "Token can't be parsed");
		TraceEvent te(SevWarn, "InvalidToken");
		te.detail("From", peer);
		te.detail("Reason", "ParseError");
		te.detail("ErrorDetail", err.get());
		if (signInput.empty()) { // unrecognizable token structure
			te.detail("Token", token.toString());
		} else { // trace with signature part taken out
			te.detail("SignInput", signInput.toString());
		}
		return false;
	}
	auto key = FlowTransport::transport().getPublicKeyByName(t.keyId);
	if (!key.present()) {
		CODE_PROBE(true, "Token referencing non-existing key");
		TRACE_INVALID_PARSED_TOKEN("UnknownKey", t);
		return false;
	} else if (!t.issuedAtUnixTime.present()) {
		CODE_PROBE(true, "Token has no issued-at field");
		TRACE_INVALID_PARSED_TOKEN("NoIssuedAt", t);
		return false;
	} else if (!t.expiresAtUnixTime.present()) {
		CODE_PROBE(true, "Token has no expiration time");
		TRACE_INVALID_PARSED_TOKEN("NoExpirationTime", t);
		return false;
	} else if (double(t.expiresAtUnixTime.get()) <= currentTime) {
		CODE_PROBE(true, "Expired token");
		TRACE_INVALID_PARSED_TOKEN("Expired", t);
		return false;
	} else if (!t.notBeforeUnixTime.present()) {
		CODE_PROBE(true, "Token has no not-before field");
		TRACE_INVALID_PARSED_TOKEN("NoNotBefore", t);
		return false;
	} else if (double(t.notBeforeUnixTime.get()) > currentTime) {
		CODE_PROBE(true, "Token's not-before is in the future");
		TRACE_INVALID_PARSED_TOKEN("TokenNotYetValid", t);
		return false;
	} else if (!t.tenants.present()) {
		CODE_PROBE(true, "Token with no tenants");
		TRACE_INVALID_PARSED_TOKEN("NoTenants", t);
		return false;
	}
	std::tie(verifyOutcome, err) = authz::jwt::verifyToken(signInput, t, key.get());
	if (err.present()) {
		CODE_PROBE(true, "Error while verifying token");
		TRACE_INVALID_PARSED_TOKEN("ErrorWhileVerifyingToken", t).detail("ErrorDetail", err.get());
		return false;
	} else if (!verifyOutcome) {
		CODE_PROBE(true, "Token with invalid signature");
		TRACE_INVALID_PARSED_TOKEN("InvalidSignature", t);
		return false;
	} else {
		CacheEntry c;
		c.expirationTime = t.expiresAtUnixTime.get();
		c.tenants.reserve(c.arena, t.tenants.get().size());
		for (auto tenantId : t.tenants.get()) {
			c.tenants.push_back(c.arena, tenantId);
		}
		if (t.tokenId.present()) {
			c.tokenId = StringRef(c.arena, t.tokenId.get());
		}
		cache.insert(StringRef(c.arena, token), c);
		return true;
	}
}

bool TokenCacheImpl::validate(TenantId tenantId, StringRef token) {
	NetworkAddress peer = FlowTransport::transport().currentDeliveryPeerAddress();
	auto cachedEntry = cache.get(token);
	double currentTime = g_network->timer();

	if (!cachedEntry.present()) {
		if (validateAndAdd(currentTime, token, peer)) {
			cachedEntry = cache.get(token);
		} else {
			return false;
		}
	}

	ASSERT(cachedEntry.present());

	auto& entry = cachedEntry.get();
	if (entry->expirationTime < currentTime) {
		CODE_PROBE(true, "Found expired token in cache");
		TraceEvent(SevWarn, "InvalidToken"_audit).detail("From", peer).detail("Reason", "ExpiredInCache");
		return false;
	}
	bool tenantFound = false;
	for (auto const& t : entry->tenants) {
		if (t == tenantId) {
			tenantFound = true;
			break;
		}
	}
	if (!tenantFound) {
		CODE_PROBE(true, "Valid token doesn't reference tenant");
		TraceEvent(SevWarn, "InvalidToken"_audit)
		    .detail("From", peer)
		    .detail("Reason", "TenantTokenMismatch")
		    .detail("RequestedTenant", fmt::format("{:#x}", tenantId))
		    .detail("TenantsInToken", fmt::format("{:#x}", fmt::join(entry->tenants, " ")));
		return false;
	}
	// audit logging
	if (FLOW_KNOBS->AUDIT_LOGGING_ENABLED)
		logTokenUsage(currentTime, AuditEntry(peer, tenantId, *cachedEntry.get()));
	return true;
}

void TokenCacheImpl::logTokenUsage(double currentTime, AuditEntry&& entry) {
	if (currentTime > lastResetTime + FLOW_KNOBS->AUDIT_TIME_WINDOW) {
		// clear usage cache every AUDIT_TIME_WINDOW seconds
		usedTokens.clear();
		lastResetTime = currentTime;
	}
	auto [iter, inserted] = usedTokens.insert(std::move(entry));
	if (inserted) {
		// access in the context of this (client_ip, tenant, token_id) tuple hasn't been logged in current window. log
		// usage.
		CODE_PROBE(true, "Audit Logging Running");
		TraceEvent("AuditTokenUsed"_audit)
		    .detail("Client", iter->address)
		    .detail("TenantId", fmt::format("{:#x}", iter->tenantId))
		    .detail("TokenId", iter->tokenId)
		    .log();
	}
}

namespace authz::jwt {
extern TokenRef makeRandomTokenSpec(Arena&, IRandom&, authz::Algorithm);
}

															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase339()
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
template <class FlowTestCase339Actor>
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
class FlowTestCase339ActorState {
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
public:
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	FlowTestCase339ActorState(UnitTestParameters const& params) 
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		 : params(params)
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase339", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase339ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase339", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto const pubKeyName = "someEcPublicKey"_sr;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto const rsaPubKeyName = "someRsaPublicKey"_sr;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto privateKey = mkcert::makeEcP256();
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto publicKey = privateKey.toPublic();
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto rsaPrivateKey = mkcert::makeRsa4096Bit();
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto rsaPublicKey = rsaPrivateKey.toPublic();
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			std::pair<std::function<void(Arena&, IRandom&, authz::jwt::TokenRef&)>, char const*> badMutations[]{ { [](Arena&, IRandom&, authz::jwt::TokenRef&) { FlowTransport::transport().removeAllPublicKeys(); }, "NoKeyWithSuchName", }, { [](Arena&, IRandom&, authz::jwt::TokenRef& token) { token.expiresAtUnixTime.reset(); }, "NoExpirationTime", }, { [](Arena&, IRandom& rng, authz::jwt::TokenRef& token) { token.expiresAtUnixTime = std::max<double>(g_network->timer() - 10 - rng.random01() * 50, 0); }, "ExpiredToken", }, { [](Arena&, IRandom&, authz::jwt::TokenRef& token) { token.notBeforeUnixTime.reset(); }, "NoNotBefore", }, { [](Arena&, IRandom& rng, authz::jwt::TokenRef& token) { token.notBeforeUnixTime = g_network->timer() + 10 + rng.random01() * 50; }, "TokenNotYetValid", }, { [](Arena&, IRandom&, authz::jwt::TokenRef& token) { token.issuedAtUnixTime.reset(); }, "NoIssuedAt", }, { [](Arena& arena, IRandom&, authz::jwt::TokenRef& token) { token.tenants.reset(); }, "NoTenants", }, { [](Arena& arena, IRandom&, authz::jwt::TokenRef& token) { TenantId* newTenants = new (arena) TenantId[1]; *newTenants = token.tenants.get()[0] + 1; token.tenants = VectorRef<TenantId>(newTenants, 1); }, "UnmatchedTenant", }, { [rsaPubKeyName](Arena& arena, IRandom&, authz::jwt::TokenRef& token) { token.keyId = rsaPubKeyName; }, "UnmatchedSignAlgorithm", }, };
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			auto const numBadMutations = sizeof(badMutations) / sizeof(badMutations[0]);
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			for(auto repeat = 0;repeat < 50;repeat++) {
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				auto arena = Arena();
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				auto& rng = *deterministicRandom();
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				auto validTokenSpec = authz::jwt::makeRandomTokenSpec(arena, rng, authz::Algorithm::ES256);
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				validTokenSpec.keyId = pubKeyName;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				for(auto i = 0;i <= numBadMutations + 1;i++) {
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					FlowTransport::transport().addPublicKey(pubKeyName, publicKey);
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					FlowTransport::transport().addPublicKey(rsaPubKeyName, rsaPublicKey);
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					auto publicKeyClearGuard = ScopeExit([]() { FlowTransport::transport().removeAllPublicKeys(); });
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					auto signedToken = StringRef();
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					auto tmpArena = Arena();
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
					if (i < numBadMutations)
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
					{
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						auto [mutationFn, mutationDesc] = badMutations[i];
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						auto mutatedTokenSpec = validTokenSpec;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						mutationFn(tmpArena, rng, mutatedTokenSpec);
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						signedToken = authz::jwt::signToken(tmpArena, mutatedTokenSpec, privateKey);
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						if (TokenCache::instance().validate(validTokenSpec.tenants.get()[0], signedToken))
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
						{
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							fmt::print("Unexpected successful validation at mutation {}, token spec: {}\n", mutationDesc, toStringRef(tmpArena, mutatedTokenSpec).toStringView());
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							ASSERT(false);
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
						}
					}
					else
					{
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
						if (i == numBadMutations)
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
						{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							signedToken = authz::jwt::signToken(tmpArena, validTokenSpec, privateKey);
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							signedToken.popBack();
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							if (TokenCache::instance().validate(validTokenSpec.tenants.get()[0], signedToken))
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
							{
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
								fmt::print("Unexpected successful validation with a token with truncated signature part\n");
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
								ASSERT(false);
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
							}
						}
						else
						{
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							auto signInput = authz::jwt::makeSignInput(tmpArena, validTokenSpec);
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							auto b64Header = signInput.eat("."_sr);
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							auto payload = base64::url::decode(tmpArena, signInput).get();
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							rapidjson::Document d;
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							d.Parse(reinterpret_cast<const char*>(payload.begin()), payload.size());
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							ASSERT(!d.HasParseError());
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							rapidjson::StringBuffer wrBuf;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							rapidjson::Writer<rapidjson::StringBuffer> wr(wrBuf);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							auto tenantsField = d.FindMember("tenants");
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							ASSERT(tenantsField != d.MemberEnd());
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							tenantsField->value.PushBack("ABC#", d.GetAllocator());
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							d.Accept(wr);
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							auto b64ModifiedPayload = base64::url::encode( tmpArena, StringRef(reinterpret_cast<const uint8_t*>(wrBuf.GetString()), wrBuf.GetSize()));
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							signInput = b64Header.withSuffix("."_sr, tmpArena).withSuffix(b64ModifiedPayload, tmpArena);
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							signedToken = authz::jwt::signToken(tmpArena, signInput, validTokenSpec.algorithm, privateKey);
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
							if (TokenCache::instance().validate(validTokenSpec.tenants.get()[0], signedToken))
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
							{
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
								fmt::print( "Unexpected successful validation of a token with tenant name containing non-base64 chars)\n");
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
								ASSERT(false);
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
							}
						}
					}
				}
			}
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (TokenCache::instance().validate(TenantInfo::INVALID_TENANT, StringRef()))
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			{
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				fmt::print("Unexpected successful validation of ill-formed token (no signature part)\n");
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				ASSERT(false);
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			}
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (TokenCache::instance().validate(TenantInfo::INVALID_TENANT, "1111.22"_sr))
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				fmt::print("Unexpected successful validation of ill-formed token (no signature part)\n");
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				ASSERT(false);
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			}
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (TokenCache::instance().validate(TenantInfo::INVALID_TENANT, "////.////.////"_sr))
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			{
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				fmt::print("Unexpected successful validation of unparseable token\n");
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				ASSERT(false);
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			}
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			fmt::print("TEST OK\n");
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (!static_cast<FlowTestCase339Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase339ActorState(); static_cast<FlowTestCase339Actor*>(this)->destroy(); return 0; }
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			new (&static_cast<FlowTestCase339Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase339ActorState();
			static_cast<FlowTestCase339Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase339ActorState();
		static_cast<FlowTestCase339Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	UnitTestParameters params;
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase339()
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
class FlowTestCase339Actor final : public Actor<Void>, public FastAllocated<FlowTestCase339Actor>, public FlowTestCase339ActorState<FlowTestCase339Actor> {
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase339Actor>::operator new;
	using FastAllocated<FlowTestCase339Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	FlowTestCase339Actor(UnitTestParameters const& params) 
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase339ActorState<FlowTestCase339Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase339", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase339");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase339", reinterpret_cast<unsigned long>(this), -1);

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
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
static Future<Void> flowTestCase339( UnitTestParameters const& params ) {
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	return Future<Void>(new FlowTestCase339Actor(params));
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase339, "/fdbrpc/authz/TokenCache/BadTokens")

#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"

															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase465()
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
template <class FlowTestCase465Actor>
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
class FlowTestCase465ActorState {
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
public:
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	FlowTestCase465ActorState(UnitTestParameters const& params) 
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		 : params(params),
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		   arena(),
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		   privateKey(mkcert::makeEcP256()),
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		   pubKeyName("somePublicKey"_sr),
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		   publicKeyClearGuard([pubKeyName = pubKeyName]() { FlowTransport::transport().removePublicKey(pubKeyName); }),
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		   tokenSpec(authz::jwt::makeRandomTokenSpec(arena, *deterministicRandom(), authz::Algorithm::ES256)),
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		   signedToken()
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase465", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase465ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase465", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			FlowTransport::transport().addPublicKey(pubKeyName, privateKey.toPublic());
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			tokenSpec.expiresAtUnixTime = g_network->timer() + 2.0;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			tokenSpec.keyId = pubKeyName;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			signedToken = authz::jwt::signToken(arena, tokenSpec, privateKey);
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (!TokenCache::instance().validate(tokenSpec.tenants.get()[0], signedToken))
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			{
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				fmt::print("Unexpected failed token validation, token spec: {}, now: {}\n", toStringRef(arena, tokenSpec).toStringView(), g_network->timer());
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
				ASSERT(false);
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			}
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(3.5);
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			if (static_cast<FlowTestCase465Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase465Actor*>(this)->actor_wait_state = 1;
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase465Actor, 0, Void >*>(static_cast<FlowTestCase465Actor*>(this)));
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
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
		this->~FlowTestCase465ActorState();
		static_cast<FlowTestCase465Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		if (TokenCache::instance().validate(tokenSpec.tenants.get()[0], signedToken))
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			fmt::print( "Unexpected successful token validation after supposedly expiring in cache, token spec: {}, now: {}\n", toStringRef(arena, tokenSpec).toStringView(), g_network->timer());
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			ASSERT(false);
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		}
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		fmt::print("TEST OK\n");
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		if (!static_cast<FlowTestCase465Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase465ActorState(); static_cast<FlowTestCase465Actor*>(this)->destroy(); return 0; }
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		new (&static_cast<FlowTestCase465Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase465ActorState();
		static_cast<FlowTestCase465Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		if (TokenCache::instance().validate(tokenSpec.tenants.get()[0], signedToken))
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			fmt::print( "Unexpected successful token validation after supposedly expiring in cache, token spec: {}, now: {}\n", toStringRef(arena, tokenSpec).toStringView(), g_network->timer());
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
			ASSERT(false);
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		}
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		fmt::print("TEST OK\n");
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
		if (!static_cast<FlowTestCase465Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase465ActorState(); static_cast<FlowTestCase465Actor*>(this)->destroy(); return 0; }
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		new (&static_cast<FlowTestCase465Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase465ActorState();
		static_cast<FlowTestCase465Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase465Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase465Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase465Actor*>(this)->ActorCallback< FlowTestCase465Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase465Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase465", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase465", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase465Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase465", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase465", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase465Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase465", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase465", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	UnitTestParameters params;
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	Arena arena;
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	PrivateKey privateKey;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	StringRef pubKeyName;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	ScopeExit<std::function<void()>> publicKeyClearGuard;
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	authz::jwt::TokenRef tokenSpec;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	StringRef signedToken;
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase465()
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
class FlowTestCase465Actor final : public Actor<Void>, public ActorCallback< FlowTestCase465Actor, 0, Void >, public FastAllocated<FlowTestCase465Actor>, public FlowTestCase465ActorState<FlowTestCase465Actor> {
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase465Actor>::operator new;
	using FastAllocated<FlowTestCase465Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase465Actor, 0, Void >;
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	FlowTestCase465Actor(UnitTestParameters const& params) 
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase465ActorState<FlowTestCase465Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase465", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase465");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase465", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase465Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
static Future<Void> flowTestCase465( UnitTestParameters const& params ) {
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
	return Future<Void>(new FlowTestCase465Actor(params));
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase465, "/fdbrpc/authz/TokenCache/GoodTokens")

#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/TokenCache.actor.cpp"
