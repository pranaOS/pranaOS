#!/usr/bin/env bash

set -e

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "${script_path}/.." || exit 1

ports=true
if [ "$1" == "--no-ports" ]; then
  ports=false
  shift
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' 

FAILURES=0

set +e

for cmd in \
        meta/check-ak-test-files.sh \
        meta/check-debug-flags.sh \
        meta/check-newlines-at-eof.py \
        meta/check-style.sh \
        meta/lint-executable-resources.sh \
        meta/lint-ipc-ids.sh \
        meta/lint-keymaps.py \
        meta/lint-shell-scripts.sh \
        meta/lint-prettier.sh \
        meta/lint-python.sh; do
    echo "Running ${cmd}... "
    if "${cmd}" "$@"; then
        echo -e "[${GREEN}OK${NC}]: ${cmd}"
    else
        echo -e "[${RED}FAIL${NC}]: ${cmd}"
        ((FAILURES+=1))
    fi
done

echo "Running meta/lint-clang-format.sh"
if meta/lint-clang-format.sh --overwrite-inplace "$@" && git diff --exit-code; then
    echo -e "[${GREEN}OK${NC}]: meta/lint-clang-format.sh"
else
    echo -e "[${RED}FAIL${NC}]: meta/lint-clang-format.sh"
    ((FAILURES+=1))
fi

if [ "$ports" = true ]; then
    if meta/lint-ports.py; then
        echo -e "[${GREEN}OK${NC}]: meta/lint-ports.py"
    else
        echo -e "[${RED}FAIL${NC}]: meta/lint-ports.py"
        ((FAILURES+=1))
    fi
fi

echo "(Not running lint-missing-resources.sh due to high false-positive rate.)"
echo "(Also look out for check-symbols.sh, which can only be executed after the build!)"

exit "${FAILURES}"