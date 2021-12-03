#!/bin/sh

set -e

dir=$(dirname "$0")

rm -f "$dir/../target/x86_64-pranaos/release/bootimage-pranaos.bin.lock"
rm -f "$dir/../disk.img.lock"

cd "$dir" && bochs -qf "bochs.rc"