#!/usr/bin/env bash

set -eo pipefail

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "${script_path}/.."

MISSING_FLAGS=n

while IFS= read -r FLAG; do
    if [ "$FLAG" = "ELF_DEBUG" ]; then
        continue
    fi

    if ! grep -qP "set\(${FLAG}" meta/CMake/all_the_debug_macros.cmake ; then
        echo "'all_the_debug_macros.cmake' is missing ${FLAG}"
        MISSING_FLAGS=y
    fi
done < <(
    git ls-files -- \
        '*.cpp' \
        '*.h' \
        '*.in' \
        ':!:kernel/filesystem/ext2_fs.h' \
    | xargs grep -P '(_DEBUG|DEBUG_)' \
    | sed -re 's,^.*[^a-zA-Z0-9_]([a-zA-Z0-9_]*DEBUG[a-zA-Z0-9_]*).*$,\1,' \
    | sort \
    | uniq
)

if [ "n" != "${MISSING_FLAGS}" ] ; then
    echo "Some flags are missing for the ALL_THE_DEBUG_MACROS feature."
    echo "If you just added a new SOMETHING_DEBUG flag,"
    echo "We want to enable all of these in automated builds, so that the code doesn't rot."
    echo "Please add it to meta/CMake/all_the_debug_macros.cmake"
    exit 1
fi