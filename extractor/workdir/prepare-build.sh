#! /bin/bash

set -ex

WORKDIR="$(realpath "$(dirname "$0")")"

FOUNDATIONDB_GIT=https://github.com/apple/foundationdb.git
FOUNDATIONDB_TAG=${FOUNDATIONDB_TAG:?FOUNDATIONDB_TAG is required}
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
git apply "${WORKDIR}/fix-fmt-format.patch"

# Configure build
# rm -rf "${FOUNDATIONDB_BUILDDIR}"
cmake -B "${FOUNDATIONDB_BUILDDIR}" -G Ninja "${FOUNDATIONDB_REPO}"
