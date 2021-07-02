#!/usr/bin/env -S bash ../.port_include.sh
port=binutils
version=2.36.1
useconfigure=true
configopts="--target=${PRANAOS_ARCH}-pc-pranaos --with-sysroot=/ --with-build-sysroot=${PRANAOS_INSTALL_ROOT} --disable-werror --disable-gdb --disable-nls"
files="https://ftpmirror.gnu.org/gnu/binutils/binutils-${version}.tar.xz binutils-${version}.tar.xz
https://ftpmirror.gnu.org/gnu/binutils/binutils-${version}.tar.xz.sig binutils-${version}.tar.xz.sig
https://ftpmirror.gnu.org/gnu/gnu-keyring.gpg gnu-keyring.gpg"
auth_type="sig"
auth_opts="--keyring ./gnu-keyring.gpg binutils-${version}.tar.xz.sig"

export ac_cv_func_getrusage=no
