#! /bin/bash

set -ex

WORKDIR="$(realpath "$(dirname "$0")")"

FOUNDATIONDB_GIT=https://github.com/apple/foundationdb.git
FOUNDATIONDB_TAG=7.1.41
FOUNDATIONDB_REPO="${WORKDIR}/foundationdb_ep"
FOUNDATIONDB_BUILDDIR="${WORKDIR}/foundationdb_build"

OUTPUT_DIR="/src"

cd "${WORKDIR}"

# Download foundationdb source

[ -d "$FOUNDATIONDB_REPO" ] || mkdir "$FOUNDATIONDB_REPO"
cd "$FOUNDATIONDB_REPO"
[ -d ".git" ] || git init
git fetch "$FOUNDATIONDB_GIT" "$FOUNDATIONDB_TAG" --tags
git checkout "$FOUNDATIONDB_TAG"

# Configure build
rm -rf "${FOUNDATIONDB_BUILDDIR}"
cmake -B "${FOUNDATIONDB_BUILDDIR}" -G Ninja "${FOUNDATIONDB_REPO}"

# Build compilers
cd "${FOUNDATIONDB_BUILDDIR}"
cmake --build . --target actorcompiler vexillographer
bash