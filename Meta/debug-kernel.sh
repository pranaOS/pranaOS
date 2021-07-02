#!/bin/sh

# Set this environment variable to override the default debugger.
#
[ -z "$PRANAOS_KERNEL_DEBUGGER" ] && PRANAOS_KERNEL_DEBUGGER="gdb"

# The QEMU -s option (enabled by default in ./run) sets up a debugger
# remote on localhost:1234. So point our debugger there, and inform
# the debugger which binary to load symbols, etc from.
#
exec $PRANAOS_KERNEL_DEBUGGER \
    -ex "file $(dirname "$0")/../Build/${PRANAOS_ARCH:-i686}/Kernel/Kernel" \
    -ex 'set arch i386:intel' \
    -ex 'target remote localhost:1234' \
    -ex "source $(dirname "$0")/PRANAOS_gdb.py" \
    "$@"
