#!/bin/bash
set -e

COMPILER_TARGET=x86_64-pranaos
COMPILER_PREFIX="local"
COMPILER_SYSROOT="../sysroot"

BINUTILS_VERSION=2.35
BINUTILS_WGET_LOC=binutils.tar.gz
BINUTILS_DIR=binutils

GCC_VERSION=10.2.0
GCC_WGET_LOC=gcc.tar.gz
GCC_DIR=gcc

DIR="$(dirname $(readlink -f $0))"

cd $DIR

if [ ! -e $BINUTILS_WGET_LOC ]; then
	echo Downloading binutils
	wget -O $BINUTILS_WGET_LOC "https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz"
else
	echo "$BINUTILS_WGET_LOC already exists, skipping download"
fi

if [ ! -e $GCC_WGET_LOC ]; then
	echo Downloading GCC
	wget -O $GCC_WGET_LOC "https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-$GCC_VERSION.tar.gz"
else
	echo "$GCC_WGET_LOC already exists, skipping download"
fi

if [ ! -e $BINUTILS_DIR ]; then
	echo Extracting binutils
	tar -xvzf binutils.tar.gz
	mv binutils-$BINUTILS_VERSION binutils
else
	echo "binutils has already been extracted, skipping"
fi

if [ ! -e $GCC_DIR ]; then
	echo Extracting GCC
	tar -xvzf gcc.tar.gz
	mv gcc-$GCC_VERSION gcc
else
	echo "GCC has already been extracted, skipping"
fi

echo Patching binutils -
./patches/patch.sh --source_directory=$BINUTILS_DIR --patch_directory=patches/binutils

echo Patching GCC -
./patches/patch.sh --source_directory=$GCC_DIR --patch_directory=patches/gcc

mkdir -p $COMPILER_PREFIX

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$(nproc)
fi

if [ ! -d "build" ]; then
	mkdir build
fi

if [ ! -d "build/binutils" ]; then
	echo "Compiling binutils"
	mkdir -p "build/binutils"
	pushd "build/binutils"
		echo "Configuring binutils"
		"$DIR/$BINUTILS_DIR/configure" \
			--target=$COMPILER_TARGET \
			--prefix="$DIR/$COMPILER_PREFIX" \
			--disable-werror \
			--disable-nls \
			--with-sysroot="$DIR/$COMPILER_SYSROOT" || exit 1

		echo "Running make on binutils"
		make
		echo "Running make install on binutils"
		make install
	popd
else
	echo "build/binutils already exists, skipping";
fi

if [ ! -d "build/gcc" ]; then
	echo "Compiling GCC"
	mkdir -p "build/gcc"
	pushd "build/gcc"
		echo "Configuring GCC"
		# You must provide the full path when using the configure script
		"$DIR/$GCC_DIR/configure" \
				--target=$COMPILER_TARGET \
				--prefix="$DIR/$COMPILER_PREFIX" \
				--disable-nls \
				--without-headers \
				--enable-languages=c,c++ \
				--disable-bootstrap \
				--with-newlib \
				--with-sysroot=$DIR/$COMPILER_SYSROOT
		# make -C "$DIR/../" install-headers || exit 1

		echo "Running make all-gcc on GCC"
		make -j $MAKEJOBS all-gcc

		echo "Running make all-target-libgcc on GCC"
		make -j $MAKEJOBS all-target-libgcc

		echo "Running make install-gcc on GCC"
		make install-gcc

		echo "Running make install-target-libgcc on GCC"
		make install-target-libgcc

		make -C $(DIR)/.. build-sysroot

		# echo "Compiling libstdc++"
		# make all-target-libstdc++-v3

		# echo "Installing libstdc++"
		# make install-target-libstdc++-v3
	popd
    else
        echo "build/gcc already exists, skipping";
    fi
