#!/usr/bin/env bash

set -eo pipefail

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "${script_path}/.."

SYSCALLS_KERNEL="$(echo 'kernel syscalls'; echo; grep -Pio '(?<=^    S\().*(?=\)( +\\)?$)' kernel/api/Syscall.h | sort)"
SYSCALLS_UE="$(echo 'Implemented in userspaceemulator'; echo; grep -Pio '(?<=^    case SC_).*(?=:$)' userland/devtools/userspaceemulator/Emulator.cpp | sort)"
SYSCALLS_MAN2="$(echo 'Documented syscalls'; echo; find Base/usr/share/man/man2/ ! -type d -printf '%f\n' | sed -Ee 's,\.md,,' | sort)"

set +e

echo "ACTUAL versus UE"
diff --color=always -u <(echo "${SYSCALLS_KERNEL}") <(echo "${SYSCALLS_UE}")
echo
echo "ACTUAL versus UE"
diff --color=always -u <(echo "${SYSCALLS_KERNEL}") <(echo "${SYSCALLS_MAN2}")

exit 0