#pragma once

#include <cstdint>
#include <foundationdb/fdb_c.h>
#include <memory>
#include <string_view>

/// Expand std::string to key parameters.
/// In the FoundationDB C API, key are expressed as sequential parameters.
#define FDB_KEY_FROM_STRING(str)                                               \
  reinterpret_cast<const uint8_t *>((str).data()),                             \
      static_cast<int>((str).size())

/// Expand std::string to value parameters
/// In the FoundationDB C API, value are expressed as sequential parameters.
#define FDB_VALUE_FROM_STRING(str)                                             \
  reinterpret_cast<const uint8_t *>((str).data()),                             \
      static_cast<int>((str).size())
#define FDB_VALUE_FROM_POD(pod)                                                \
  reinterpret_cast<const uint8_t *>(&pod), sizeof(pod)

#define FDB_KEYSEL_LAST_LESS_THAN_STRING(str)                                  \
  FDB_KEYSEL_LAST_LESS_THAN(reinterpret_cast<const uint8_t *>((str).data()),   \
                            static_cast<int>((str).size()))
#define FDB_KEYSEL_LAST_LESS_OR_EQUAL_STRING(str)                              \
  FDB_KEYSEL_LAST_LESS_OR_EQUAL(                                               \
      reinterpret_cast<const uint8_t *>((str).data()),                         \
      static_cast<int>((str).size()))
#define FDB_KEYSEL_FIRST_GREATER_THAN_STRING(str)                              \
  FDB_KEYSEL_FIRST_GREATER_THAN(                                               \
      reinterpret_cast<const uint8_t *>((str).data()),                         \
      static_cast<int>((str).size()))
#define FDB_KEYSEL_FIRST_GREATER_OR_EQUAL_STRING(str)                          \
  FDB_KEYSEL_FIRST_GREATER_OR_EQUAL(                                           \
      reinterpret_cast<const uint8_t *>((str).data()),                         \
      static_cast<int>((str).size()))

template <class FDBObject>
constexpr inline void (*FDBObjectDeleter)(FDBObject *);

/// Manage fdb object in shared_ptr
template <class FDBObject>
inline std::shared_ptr<FDBObject> fdb_manage_object(FDBObject *obj);

#define FDB_OBJECT_DELETER(FDBObject, Deleter)                                 \
  template <>                                                                  \
  inline std::shared_ptr<FDBObject> fdb_manage_object(FDBObject *obj) {        \
    return std::shared_ptr<FDBObject>(obj, Deleter);                           \
  }

FDB_OBJECT_DELETER(FDBDatabase, fdb_database_destroy);
FDB_OBJECT_DELETER(FDBTransaction, fdb_transaction_destroy);
FDB_OBJECT_DELETER(FDBFuture, fdb_future_destroy);
