#!/usr/bin/env bash

set -eo pipefail

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "${script_path}/.."

MISSING_FILES=n

while IFS= read -r FILENAME; do
    if ! grep -qP "${FILENAME}" base/tests/CMakeLists.txt ; then
        echo "Tests/base/CMakeLists.txt is missing the test file ${FILENAME}"
        MISSING_FILES=y
    fi
done < <(
    git ls-files 'Test/base/Test*.cpp' | xargs -i basename {}
)

if [ "n" != "${MISSING_FILES}" ] ; then
    echo "Some files are missing from the base/tests/CMakeLists.txt."
    echo "If a new test file is being added, ensure it is in the CMakeLists.txt."
    exit 1
fi