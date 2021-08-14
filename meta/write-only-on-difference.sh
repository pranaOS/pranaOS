#!/bin/sh

set -e

if [ "$#" -lt "2" ]; then
    echo "USAGE: $0 <file> <cmd...>"
    exit 1
fi

DST_FILE="$1"
shift

mkdir -p -- "$(dirname -- "${DST_FILE}")"

cleanup()
{
  rm -f -- "${DST_FILE}.tmp"
}
trap cleanup 0 1 2 3 6

"$@" > "${DST_FILE}.tmp"

if ! cmp -s -- "${DST_FILE}.tmp" "${DST_FILE}"; then

    mv -f -- "${DST_FILE}.tmp" "${DST_FILE}"
fi
