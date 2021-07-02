#!/usr/bin/env -S bash ../.port_include.sh
port=cmatrix
version=2.0
useconfigure=true
configopts="--target=${PRANAOS_ARCH}-pc-pranaos --with-sysroot=/ --with-build-sysroot=${PRANAOS_INSTALL_ROOT} --disable-werror --disable-gdb --disable-nls"
files="https://github.com/abishekvashok/cmatrix/releases/download/v2.0/cmatrix-v2.0-Butterscotch.tar"
auth_type="sig"
auth_opts="--keyring ./gnu-keyring.gpg binutils-${version}.tar.xz.sig"
export ac_cv_func_getrusage=no
