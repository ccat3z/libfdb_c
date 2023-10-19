#! /bin/bash

set -ex

WORKDIR="$(realpath "$(dirname "$0")")"

FOUNDATIONDB_GIT=https://github.com/apple/foundationdb.git
FOUNDATIONDB_TAG=${FOUNDATIONDB_TAG:?FOUNDATIONDB_TAG is required}
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
# rm -rf "${FOUNDATIONDB_BUILDDIR}"
cmake -B "${FOUNDATIONDB_BUILDDIR}" -G Ninja "${FOUNDATIONDB_REPO}"

# Build compilers
cd "${FOUNDATIONDB_BUILDDIR}"
cmake --build . --target actorcompiler vexillographer
ACTORCOMPILER="mono ${FOUNDATIONDB_BUILDDIR}/actorcompiler.exe"
VEXILLOGRAPHER="mono ${FOUNDATIONDB_BUILDDIR}/vexillographer.exe"

# Copy flow source
[ -d "${OUTPUT_DIR}/flow" ] || mkdir "${OUTPUT_DIR}/flow"
flow_skip_source=(
    "CMakeLists.txt"
    "README.md"
    "stacktrace_internal"
    "coveragetool"
    "actorcompiler"
    "no_intellisense.opt"
)
for path in "${FOUNDATIONDB_REPO}/flow/"*; do
    file="$(basename "$path")"
    if [[ " ${flow_skip_source[@]} " =~ " $file " ]]; then
        echo "Skip $file"
    elif [[ "$file" =~ ^(.*)\.actor\.(h|cpp)$ ]]; then
        $ACTORCOMPILER "$path" "${OUTPUT_DIR}/flow/${BASH_REMATCH[1]}.actor.g.${BASH_REMATCH[2]}"
    elif [[ "$file" =~ ^(.*)\.(h|cpp)$ ]]; then
        sed 's/\.actor\.h/.actor.g.h/' "$path" > "${OUTPUT_DIR}/flow/${file}"
    elif [ "$file" = "config.h.cmake" ]; then
        cp "$path" "${OUTPUT_DIR}/flow/${file}"
    elif [[ "$file" =~ ^(.*).h.cmake$ ]]; then
        cp "$FOUNDATIONDB_BUILDDIR/flow/${BASH_REMATCH[1]}.h" "${OUTPUT_DIR}/flow/${BASH_REMATCH[1]}.h"
    else
        cp -R "$path" "${OUTPUT_DIR}/flow/${file}"
    fi
done
