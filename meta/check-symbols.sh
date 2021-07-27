#!/bin/sh

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "$script_path/.." || exit 1


FORBIDDEN_SYMBOLS="__cxa_guard_acquire __cxa_guard_release"
LIBC_PATH="build/userland/libraries/libc/libc.a"
for forbidden_symbol in $FORBIDDEN_SYMBOLS; do
    nm $LIBC_PATH | grep "U $forbidden_symbol"
    APPEARS_AS_UNDEFINED=$?
    nm $LIBC_PATH | grep "T $forbidden_symbol"
    APPEARS_AS_DEFINED=$?
    if [ $APPEARS_AS_UNDEFINED -eq 0 ] && [ ! $APPEARS_AS_DEFINED -eq 0 ]; then
        echo "Forbidden undefined symbol in libc: $forbidden_symbol"
        echo "See comment in meta/check-symbols.sh for more info"
        exit 1
    fi
done