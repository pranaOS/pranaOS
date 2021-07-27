#!/bin/sh

[ -z "$PRANAOS_KERNEL_DEBUGGER" ] && PRANAOS_KERNEL_DEBUGGER="gdb"

if [ "$PRANAOS_ARCH" = "x86_64" ]; then
    gdb_arch=i386:x86-64
    prekernel_image=Prekernel64
    kernel_base=0x2000200000
else
    gdb_arch=i386:intel
    prekernel_image=Prekernel
    kernel_base=0xc0200000
fi

exec $PRANAOS_KERNEL_DEBUGGER \
    -ex "file $(dirname "$0")/../Build/${PRANAOS_ARCH:-i686}/kernel/prekernel/$prekernel_image" \
    -ex "set confirm off" \
    -ex "add-symbol-file $(dirname "$0")/../Build/${PRANAOS_ARCH:-i686}/kernel/kernel -o $kernel_base" \
    -ex "set confirm on" \
    -ex "set arch $gdb_arch" \
    -ex 'target remote localhost:1234' \
    -ex "source $(dirname "$0")/pranaos_gdb.py" \
    -ex "layout asm" \
    -ex "fs next" \
    "$@"