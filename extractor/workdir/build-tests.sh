#! /bin/bash

set -ex

WORKDIR="$(realpath "$(dirname "$0")")"

export FOUNDATIONDB_REPO="${WORKDIR}/foundationdb_ep"
export FOUNDATIONDB_BUILDDIR="${WORKDIR}/foundationdb_build"

# Prepare build
"${WORKDIR}/prepare-build.sh"

# Build compilers
cd "${FOUNDATIONDB_BUILDDIR}"
cmake --build . --target \
    fdb_c90_test \
    fdb_c_performance_test \
    fdb_c_ryw_benchmark \
    fdb_c_txn_size_test \
    fdb_c_client_memory_test \
    mako \
    fdb_c_setup_tests \
    fdb_c_unit_tests \
    fdb_c_unit_tests_version_510 \
    trace_partial_file_suffix_test \
    disconnected_timeout_unit_tests \
    fdb_c_api_tester
