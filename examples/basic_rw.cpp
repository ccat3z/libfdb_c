#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>

#define FDB_API_VERSION 710
#include <foundationdb/fdb_c.h>
#include <foundationdb/fdb_c_types.h>

namespace {
void assert_fdb_error(fdb_error_t error) {
  if (error) {
    std::cerr << "Err: " << fdb_get_error(error) << std::endl;
    abort();
  }
}

struct scope_guard {
  scope_guard(std::function<void()> onExit) : onExit(onExit) {}
  ~scope_guard() { onExit(); }

private:
  const std::function<void()> onExit;
};
} // namespace

int main() {
    return 0;
}
