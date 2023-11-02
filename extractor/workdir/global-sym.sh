#! /bin/bash

set -exu

WORKDIR="$(realpath "$(dirname "$0")")"

export FOUNDATIONDB_REPO="${WORKDIR}/foundationdb_ep"
export FOUNDATIONDB_BUILDDIR="${WORKDIR}/foundationdb_build"

# Prepare build
"${WORKDIR}/prepare-build.sh"

# Build compilers
cd "${FOUNDATIONDB_BUILDDIR}"
cmake --build . --target fdb_c

cd "$(dirname "$0")"
clang++-${LLVM_VERSION} -g \
    -std=c++2a \
    -I/usr/include/llvm-${LLVM_VERSION} \
    -L/usr/lib/llvm-${LLVM_VERSION}/lib \
    -o global-sym \
    global-sym.cpp \
    -lLLVMDemangle
for lib in fdb_c fdbclient fdbrpc flow; do
    nm "${FOUNDATIONDB_BUILDDIR}/lib/lib$lib.a" | sed -En '/ [Ww] /s/^[0-9a-f]* [Ww] (\S+)$/\1/p'
done | ./global-sym -s 2> global-sym.log | sort 1> global-sym.list
