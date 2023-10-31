#! /bin/bash

set -ex

WORKDIR="$(realpath "$(dirname "$0")")"

FOUNDATIONDB_GIT=https://github.com/apple/foundationdb.git
FOUNDATIONDB_TAG=7.3.25
FOUNDATIONDB_REPO=${FOUNDATIONDB_REPO:?FOUNDATIONDB_REPO is required}
FOUNDATIONDB_BUILDDIR=${FOUNDATIONDB_BUILDDIR:?FOUNDATIONDB_BUILDDIR is required}

cd "${WORKDIR}"

# Download foundationdb source
[ -d "$FOUNDATIONDB_REPO" ] || mkdir "$FOUNDATIONDB_REPO"
cd "$FOUNDATIONDB_REPO"
[ -d ".git" ] || git init
git fetch "$FOUNDATIONDB_GIT" "$FOUNDATIONDB_TAG" --tags
git checkout -f "$FOUNDATIONDB_TAG"
git reset HEAD --hard
git clean -dffx

# Apply patchs
for patch in fix-fmt-cxx20.patch # build-c-binding-only.patch fix-fmt-format.patch backport-doctest.patch
do
    git apply "${WORKDIR}/patches/${patch}"
done

# Configure build
# rm -rf "${FOUNDATIONDB_BUILDDIR}"
cmake -B "${FOUNDATIONDB_BUILDDIR}" -DCMAKE_BUILD_TYPE=Debug -DUSE_JEMALLOC=OFF -G Ninja "${FOUNDATIONDB_REPO}"
