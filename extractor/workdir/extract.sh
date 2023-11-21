#! /bin/bash

set -ex

OUTPUT_DIR="$1"
[ -d "$OUTPUT_DIR" ] || exit 1

WORKDIR="$(realpath "$(dirname "$0")")"

export FOUNDATIONDB_REPO="${WORKDIR}/foundationdb_ep"
export FOUNDATIONDB_BUILDDIR="${WORKDIR}/foundationdb_build"

# Prepare build
"${WORKDIR}/prepare-build.sh"

# Build compilers
cd "${FOUNDATIONDB_BUILDDIR}"
cmake --build . --target actorcompiler vexillographer
ACTORCOMPILER="mono ${FOUNDATIONDB_BUILDDIR}/actorcompiler.exe"
VEXILLOGRAPHER="mono ${FOUNDATIONDB_BUILDDIR}/vexillographer.exe"

# Copy flow source
function convert_fdb_source() {
    rm -rf "${OUTPUT_DIR}/${_subdir}"
    mkdir -p "${OUTPUT_DIR}/${_subdir}"

    for path in "${FOUNDATIONDB_REPO}/${_subdir}/"*; do
        file="$(basename "$path")"
        if [[ " ${_skip_source[@]} " =~ " $file " ]]; then
            echo "Skip $file"
        elif [[ " ${_copy_source[@]} " =~ " $file " ]]; then
            cp "$path" "${OUTPUT_DIR}/${_subdir}/${file}"
        elif [[ "$file" =~ ^(.*)\.(h|cpp)$ ]]; then
            _target="${OUTPUT_DIR}/${_subdir}/${file}"
            sed  "$path" \
                -e 's|contrib/fmt-8.1.1/include/||' \
                -e 's|flow/stacktrace.h|absl/debugging/stacktrace.h|' \
                -e 's|fdbrpc/md5/md5.h|openssl/md5.h|;' \
                > "$_target"

            if [[ "$file" =~ ^(.*)\.actor\.(h|cpp)$ ]]; then
                _actor_target="${OUTPUT_DIR}/${_subdir}/${BASH_REMATCH[1]}.actor.g.${BASH_REMATCH[2]}"
                $ACTORCOMPILER "$_target" "$_actor_target" --generate-probes
            fi
        elif [[ "$file" =~ ^(.*).h.cmake$ ]]; then
            cp "$FOUNDATIONDB_BUILDDIR/${_subdir}/${BASH_REMATCH[1]}.h" "${OUTPUT_DIR}/${_subdir}/${BASH_REMATCH[1]}.h"
        else
            cp -R "$path" "${OUTPUT_DIR}/${_subdir}/${file}"
        fi
    done
    cp "${WORKDIR}/cmake/${_subdir}.cmake" "${OUTPUT_DIR}/${_subdir}/CMakeLists.txt"
}

# flow
_subdir=flow
_skip_source=(
    CMakeLists.txt
    README.md
    coveragetool
    actorcompiler
    no_intellisense.opt
    stacktrace_internal
    stacktrace.amalgamation.cpp
    stacktrace.h
)
_copy_source=(
    config.h.cmake
)
convert_fdb_source

# fdbrpc
_subdir=fdbrpc
_skip_source=(
    CMakeLists.txt
    README.md
    actorFuzz.py

    # Depends
    libcoroutine # use boost coro impl
    md5          # use openssl impl
    # libb64
    # libeio
)
_copy_source=()
convert_fdb_source

# fdbrpc requires fdbserver/Knobs.h
mkdir "${OUTPUT_DIR}/fdbserver"
cp "${FOUNDATIONDB_REPO}/fdbserver/Knobs.h" "${OUTPUT_DIR}/fdbserver/Knobs.h"

# fdbclient
_subdir=fdbclient
_skip_source=(
    CMakeLists.txt
    README.md
    azurestorage.cmake
    vexillographer  

    # Depends
    rapidxml        # required by s3
    rapidjson       # required by unit test
    # json_spirit
    # sha1

    # Disable AZURE
    BackupContainerAzureBlobStore.actor.cpp

    # Disable S3
    S3BlobStore.actor.cpp
)
_copy_source=()
convert_fdb_source
$VEXILLOGRAPHER "${FOUNDATIONDB_REPO}/fdbclient/vexillographer/fdb.options" cpp "${OUTPUT_DIR}/fdbclient/FDBOptions.g"

# bindings/c
mkdir -p "${OUTPUT_DIR}/bindings/c"
cp "${FOUNDATIONDB_REPO}/bindings/c/fdb_c.cpp" "${OUTPUT_DIR}/bindings/c/fdb_c.cpp"
cp -r "${FOUNDATIONDB_REPO}/bindings/c/foundationdb" "${OUTPUT_DIR}/bindings/c/foundationdb"
$VEXILLOGRAPHER "${FOUNDATIONDB_REPO}/fdbclient/vexillographer/fdb.options" c "${OUTPUT_DIR}/bindings/c/foundationdb/fdb_c_options.g.h"
function generate_fdb_c_asm() {
    _os="${1}"
    _cpu="${2}"
    _asm_dir="${OUTPUT_DIR}/bindings/c/${_os}-${_cpu}"

    mkdir "${_asm_dir}"
    python3 "${FOUNDATIONDB_REPO}/bindings/c/generate_asm.py" \
        "${_os}" "${_cpu}" \
        "${FOUNDATIONDB_REPO}/bindings/c/fdb_c.cpp" \
        "${_asm_dir}/fdb_c.g.S" \
        "${_asm_dir}/fdb_c_function_pointers.g.h"
}
generate_fdb_c_asm linux intel
generate_fdb_c_asm linux aarch64
generate_fdb_c_asm osx aarch64
cp "${WORKDIR}/cmake/fdb_c.cmake" "${OUTPUT_DIR}/bindings/c/CMakeLists.txt"

# extended FDB api for FDBKeeper
$ACTORCOMPILER "${WORKDIR}/patches/fdb_c.actor.cpp" "${OUTPUT_DIR}/bindings/c/fdb_c.actor.g.cpp"
cp "${WORKDIR}/patches/fdb_c_ext.h" "${OUTPUT_DIR}/bindings/c/foundationdb/fdb_c_ext.h"

