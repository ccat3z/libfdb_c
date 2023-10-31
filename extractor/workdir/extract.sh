#! /bin/bash

set -exu

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

    find "${FOUNDATIONDB_REPO}/${_subdir}/" -type f -print0 | 
    while IFS= read -r -d '' path; do
        dir="$(realpath --relative-to="${FOUNDATIONDB_REPO}/${_subdir}" "$(dirname "$path")")/"
        [ "$dir" = "./" ] && dir=
        file="$(basename "$path")"

        skip=N
        copy=N

        parent_dir="${dir}${file}"
        while [ "$parent_dir" != "." ]; do
            if [[ " ${_skip_source[@]} " =~ " $parent_dir " ]]; then
                skip=Y
                break;
            fi
            parent_dir="$(dirname "$parent_dir")"
        done

        if [ "$skip" = "Y" ]; then
            echo "Skip $dir$file"
        elif [[ " ${_copy_source[@]} " =~ " $dir$file " ]]; then
            copy=Y
        elif [[ "$file" =~ ^(.*)\.(h|cpp)$ ]]; then
            _target="${OUTPUT_DIR}/${_subdir}/${dir}${file}"

            mkdir -p "$(dirname "$_target")"
            sed  "$path" \
                -e 's|contrib/fmt-8.1.1/include/||' \
                -e 's|stacktrace/stacktrace.h|absl/debugging/stacktrace.h|' \
                -e 's|md5/md5.h|openssl/md5.h|;' \
                > "$_target"

            if [[ "$file" =~ ^(.*)\.actor\.(h|cpp)$ ]]; then
                _actor_target="${OUTPUT_DIR}/${_subdir}/${dir}${BASH_REMATCH[1]}.actor.g.${BASH_REMATCH[2]}"
                $ACTORCOMPILER "$_target" "$_actor_target" --generate-probes
            fi
        elif [[ "$file" =~ ^(.*).h.cmake$ ]]; then
            _incdir="${OUTPUT_DIR}/${_subdir}/include/${_subdir}"
            mkdir -p "$_incdir"
            cp "$FOUNDATIONDB_BUILDDIR/${_subdir}/include/${_subdir}/${BASH_REMATCH[1]}.h" "${_incdir}/${BASH_REMATCH[1]}.h"
        else
            copy=Y
        fi

        if [ "$copy" = "Y" ]; then
            mkdir -p "${OUTPUT_DIR}/${_subdir}/${dir}"
            cp "$path" "${OUTPUT_DIR}/${_subdir}/${dir}${file}"
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
    ApiVersions.cmake
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
    tests

    TokenSign

    # Depends
    libcoroutine # use boost coro impl
    # libeio
)
_copy_source=()
convert_fdb_source

# fdbrpc requires fdbserver/Knobs.h
mkdir -p "${OUTPUT_DIR}/fdbserver/include/fdbserver"
cp "${FOUNDATIONDB_REPO}/fdbserver/include/fdbserver/Knobs.h" "${OUTPUT_DIR}/fdbserver/include/fdbserver/Knobs.h"

# fdbclient
_subdir=fdbclient
_skip_source=(
    CMakeLists.txt
    README.md
    azurestorage.cmake
    vexillographer  

    # Disable AZURE
    azure_backup

    # Disable S3
    S3BlobStore.actor.cpp
)
_copy_source=()
convert_fdb_source
$VEXILLOGRAPHER "${FOUNDATIONDB_REPO}/fdbclient/vexillographer/fdb.options" cpp "${OUTPUT_DIR}/fdbclient/FDBOptions.g"
mv "${OUTPUT_DIR}"/fdbclient{,/include/fdbclient}/FDBOptions.g.h
$VEXILLOGRAPHER "${FOUNDATIONDB_REPO}/fdbclient/vexillographer/fdb.options" c "${OUTPUT_DIR}/fdbclient/include/fdbclient/fdb_c_options.g.h"

# bindings/c
mkdir -p "${OUTPUT_DIR}/bindings/c"
cp "${FOUNDATIONDB_REPO}/bindings/c/fdb_c.cpp" "${OUTPUT_DIR}/bindings/c/fdb_c.cpp"
cp -r "${FOUNDATIONDB_REPO}/bindings/c/foundationdb" "${OUTPUT_DIR}/bindings/c/foundationdb"

rm "${OUTPUT_DIR}/bindings/c/foundationdb/fdb_c_apiversion.h.cmake"
cp {"${FOUNDATIONDB_BUILDDIR}","${OUTPUT_DIR}"}/bindings/c/foundationdb/fdb_c_apiversion.g.h

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

mkdir "${OUTPUT_DIR}/contrib"
for dep in crc32 SimpleOpt libb64
do
    cp -R "${FOUNDATIONDB_REPO}/contrib/${dep}" "${OUTPUT_DIR}/contrib/${dep}"
done
