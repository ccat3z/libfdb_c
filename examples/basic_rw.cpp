#include "fdb_helper.h"
#include <cstdint>
#include <cstdlib>
#include <foundationdb/fdb_c.h>
#include <foundationdb/fdb_c_types.h>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>

namespace {
void assert_fdb_error(fdb_error_t error) {
  if (error) {
    std::cerr << "Err: " << fdb_get_error(error) << std::endl;
    abort();
  }
}

class scope_guard {
public:
  scope_guard(std::function<void()> onExit) : onExit(onExit) {}
  ~scope_guard() { onExit(); }

private:
  const std::function<void()> onExit;
};

std::shared_ptr<FDBDatabase> create_database() {
  FDBDatabase *db;
  assert_fdb_error(fdb_create_database(NULL, &db));
  return fdb_manage_object(db);
}

std::shared_ptr<FDBTransaction> create_transaction(FDBDatabase &db) {
  FDBTransaction *tr;
  assert_fdb_error(fdb_database_create_transaction(&db, &tr));
  return fdb_manage_object(tr);
}

void wait(std::shared_ptr<FDBFuture> future) {
  assert_fdb_error(fdb_future_block_until_ready(future.get()));
}
} // namespace

int main() {
  using namespace std::chrono_literals;

  assert_fdb_error(fdb_select_api_version(710));

  // Start network
  assert_fdb_error(fdb_setup_network());
  std::jthread network_thread([]() { assert_fdb_error(fdb_run_network()); });
  scope_guard network_guard([]() { assert_fdb_error(fdb_stop_network()); });
  std::this_thread::sleep_for(1s);

  // Setup db
  auto db = create_database();

  std::string key = "a";
  std::string data = "1";

  // Create key
  {
    std::cerr << "set " << key << "=" << data << std::endl;
    auto tr = create_transaction(*db);

    fdb_transaction_set(tr.get(), FDB_KEY_FROM_STRING(key),
                        FDB_VALUE_FROM_STRING(data));
    auto f = fdb_manage_object(fdb_transaction_commit(tr.get()));
    wait(f);
  }

  // Read key
  {
    auto tr = create_transaction(*db);

    std::string data = "1";
    auto f = fdb_manage_object(
        fdb_transaction_get(tr.get(), FDB_KEY_FROM_STRING(key), true));
    wait(f);

    fdb_bool_t present;
    const uint8_t *value;
    int value_length;
    assert_fdb_error(
        fdb_future_get_value(f.get(), &present, &value, &value_length));

    if (present) {
      std::cerr << "get " << std::string_view((const char *)value, value_length)
                << std::endl;
    } else {
      std::cerr << "not exists" << std::endl;
    }
  }

  return 0;
}
