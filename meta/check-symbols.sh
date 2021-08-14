#!/bin/sh

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "$script_path/.." || exit 1

FORBIDDEN_SYMBOLS="__cxa_guard_acquire __cxa_guard_release"
LIBC_PATH="Build/Userland/Libraries/LibC/libc.a"
for forbidden_symbol in $FORBIDDEN_SYMBOLS; do
    # check if there's an undefined reference to the symbol & it is not defined anywhere else in the library
    nm $LIBC_PATH | grep "U $forbidden_symbol"
    APPEARS_AS_UNDEFINED=$?
    nm $LIBC_PATH | grep "T $forbidden_symbol"
    APPEARS_AS_DEFINED=$?
    if [ $APPEARS_AS_UNDEFINED -eq 0 ] && [ ! $APPEARS_AS_DEFINED -eq 0 ]; then
        echo "Forbidden undefined symbol in LibC: $forbidden_symbol"
        echo "See comment in Meta/check-symbols.sh for more info"
        exit 1
    fi
done
