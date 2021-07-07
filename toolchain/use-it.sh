#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export PATH="$DIR/local/bin:$PATH"

export PRANAOS_SOURCEROOT="$DIR/.."
export PRANAOS_TOOLCHAIN="$PRANAOS_SOURCEROOT/toolchain"
export PRANAOS_SYSROOT="$PRANAOS_SOURCEROOT/build/sysroot"
export PRANAOS_CONTRIBROOT="$PRANAOS_SOURCEROOT/contribs"

echo "$DIR/local/bin"