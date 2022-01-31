#!/bin/bash

set -Eeuo pipefail

# ============ CONFIGURATIONS ============

PREFIX="$HOME/opt/cross"
TARGET=i386-pc-pranaos

SYSROOT="$HOME/os/pranaOS/toolchain/sysroot"
JOBCOUNT=$(nproc)


# ============ INSTALLING DEPENDENCIES ============
binutils_install() {
  wget "https://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.gz"
  tar -xzvf binutils-2.32.tar.gz
}

gcc_install() {
  wget "https://ftp.gnu.org/gnu/gcc/gcc-9.1.0/gcc-9.1.0.tar.gz"
  tar -xzvf gcc-9.1.0.tar.gz
}

# ============ BUILDING ============
binutils_build() {
  cd binutils-2.32
  patch -p1 < ../patch/binutils-2.32.patch
  cd ld
  automake
  cd ../.. && mkdir build-binutils
  cd build-binutils
  ../binutils-2.32/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot=$SYSROOT --disable-werror
  make -j $JOBCOUNT
  make install
  cd ..
}

gcc_build() {
  cd gcc-9.1.0
  patch -p1 < ../patch/gcc-9.1.0.patch
  ./contrib/download_prerequisites
  cd .. && mkdir build-gcc
  cd build-gcc
  ../gcc-9.1.0/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot=$SYSROOT --enable-languages=c,c++
  make all-gcc all-target-libgcc -j $JOBCOUNT
  make install-gcc install-target-libgcc
  cd ..
}

# ============ RUNNING THE FUNCTIONS ============
binutils_fetch && binutils_build
gcc_fetch && gcc_build