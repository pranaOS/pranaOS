#!/usr/bin/env bash
set -eo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "$DIR"

ARCH=${ARCH:-"i686"}
TARGET="$ARCH-pc-pranaos"
PREFIX="$DIR/Local/$ARCH"
BUILD="$DIR/../build/$ARCH"
SYSROOT="$BUILD/root"

MAKE="make"
MD5SUM="md5sum"
NPROC="nproc"
REALPATH="realpath"

if command -v ginstall &>/dev/null; then
    INSTALL=ginstall
else
    INSTALL=install
fi

SYSTEM_NAME="$(uname -s)"

export CFLAGS="-g0 -O2 -mtune=native"
export CXXFLAGS="-g0 -O2 -mtune=native"

if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
    MAKE=gmake
    MD5SUM="md5 -q"
    NPROC="sysctl -n hw.ncpuonline"
    REALPATH="readlink -f"
    export CC=egcc
    export CXX=eg++
    export with_gmp=/usr/local
    export LDFLAGS=-Wl,-z,notext
elif [ "$SYSTEM_NAME" = "FreeBSD" ]; then
    MAKE=gmake
    MD5SUM="md5 -q"
    NPROC="sysctl -n hw.ncpu"
    export with_gmp=/usr/local
    export with_mpfr=/usr/local
fi

if [ ! -d "$BUILD" ]; then
    mkdir -p "$BUILD"
fi
BUILD=$($REALPATH "$BUILD")

git_patch=
while [ "$1" != "" ]; do
    case $1 in
        --dev )           git_patch=1
                          ;;
    esac
    shift
done

echo PREFIX is "$PREFIX"
echo SYSROOT is "$SYSROOT"

mkdir -p "$DIR/Tarballs"

BINUTILS_VERSION="2.37"
BINUTILS_MD5SUM="1e55743d73c100b7a0d67ffb32398cdb"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="${BINUTILS_NAME}.tar.gz"
BINUTILS_BASE_URL="https://ftp.gnu.org/gnu/binutils"

GDB_VERSION="10.2"
GDB_MD5SUM="7aeb896762924ae9a2ec59525088bada"
GDB_NAME="gdb-$GDB_VERSION"
GDB_PKG="${GDB_NAME}.tar.gz"
GDB_BASE_URL="https://ftp.gnu.org/gnu/gdb"

GCC_VERSION="11.2.0"
GCC_MD5SUM="dc6886bd44bb49e2d3d662aed9729278"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="${GCC_NAME}.tar.gz"
GCC_BASE_URL="https://ftp.gnu.org/gnu/gcc"

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${NAME}"$']\x1b[39m |'
}

buildstep dependencies echo "Checking whether 'make' is available..."
if ! command -v ${MAKE:-make} >/dev/null; then
    buildstep dependencies echo "Please make sure to install GNU Make (for the '${MAKE:-make}' tool)."
    exit 1
fi

buildstep dependencies echo "Checking whether 'patch' is available..."
if ! command -v patch >/dev/null; then
    buildstep dependencies echo "Please make sure to install GNU patch (for the 'patch' tool)."
    exit 1
fi

buildstep dependencies echo "Checking whether your C compiler works..."
if ! ${CC:-cc} -o /dev/null -xc - >/dev/null <<'PROGRAM'
int main() {}
PROGRAM
then
    buildstep dependencies echo "Please make sure to install a working C compiler."
    exit 1
fi

if [ "$SYSTEM_NAME" != "Darwin" ]; then
    for lib in gmp mpc mpfr; do
        buildstep dependencies echo "Checking whether the $lib library and headers are available..."
        if ! ${CC:-cc} -I /usr/local/include -L /usr/local/lib -l$lib -o /dev/null -xc - >/dev/null <<PROGRAM
#include <$lib.h>
int main() {}
PROGRAM
        then
            echo "Please make sure to install the $lib library and headers."
            exit 1
        fi
    done
fi

pushd "$DIR"
    if [ "${TRY_USE_LOCAL_TOOLCHAIN}" = "y" ] ; then

        mkdir -p Cache
        echo "Cache (before):"
        ls -l Cache
        CACHED_TOOLCHAIN_ARCHIVE="Cache/ToolchainBinariesGithubActions.tar.gz"
        if [ -r "${CACHED_TOOLCHAIN_ARCHIVE}" ] ; then
            echo "Cache at ${CACHED_TOOLCHAIN_ARCHIVE} exists!"
            echo "Extracting toolchain from cache:"
            if tar xzf "${CACHED_TOOLCHAIN_ARCHIVE}" ; then
                echo "Done 'building' the toolchain."
                echo "Cache unchanged."
                exit 0
            else
                echo
                echo
                echo
                echo "Could not extract cached toolchain archive."
                echo "This means the cache is broken and *should be removed*!"
                echo "As Github Actions cannot update a cache, this will unnecessarily"
                echo "slow down all future builds for this hash, until someone"
                echo "resets the cache."
                echo
                echo
                echo
                rm -f "${CACHED_TOOLCHAIN_ARCHIVE}"
            fi
        else
            echo "Cache at ${CACHED_TOOLCHAIN_ARCHIVE} does not exist."
            echo "Will rebuild toolchain from scratch, and save the result."
        fi
        echo "::group::Actually building Toolchain"
    fi
popd


pushd "$DIR/Tarballs"
    if [ "$ARCH" = "aarch64" ]; then
        md5=""
        if [ -e "$GDB_PKG" ]; then
            md5="$($MD5SUM $GDB_PKG | cut -f1 -d' ')"
            echo "bu md5='$md5'"
        fi
        if [ "$md5" != ${GDB_MD5SUM} ] ; then
            rm -f $GDB_PKG
            curl -LO "$GDB_BASE_URL/$GDB_PKG"
        else
            echo "Skipped downloading gdb"
        fi
    fi

    md5=""
    if [ -e "$BINUTILS_PKG" ]; then
        md5="$($MD5SUM $BINUTILS_PKG | cut -f1 -d' ')"
        echo "bu md5='$md5'"
    fi
    if [ "$md5" != ${BINUTILS_MD5SUM} ] ; then
        rm -f $BINUTILS_PKG
        curl -LO "$BINUTILS_BASE_URL/$BINUTILS_PKG"
    else
        echo "Skipped downloading binutils"
    fi

    md5=""
    if [ -e "$GCC_PKG" ]; then
        md5="$($MD5SUM ${GCC_PKG} | cut -f1 -d' ')"
        echo "gc md5='$md5'"
    fi
    if [ "$md5" != ${GCC_MD5SUM} ] ; then
        rm -f $GCC_PKG
        curl -LO "$GCC_BASE_URL/$GCC_NAME/$GCC_PKG"
    else
        echo "Skipped downloading gcc"
    fi

    if [ "$ARCH" = "aarch64" ]; then
        if [ -d ${GDB_NAME} ]; then
            rm -rf "${GDB_NAME}"
            rm -rf "$DIR/build/$ARCH/$GDB_NAME"
        fi
        echo "Extracting GDB..."
        tar -xzf ${GDB_PKG}

        pushd ${GDB_NAME}
            if [ "$git_patch" = "1" ]; then
                git init > /dev/null
                git add . > /dev/null
                git commit -am "BASE" > /dev/null
                git apply "$DIR"/Patches/gdb.patch > /dev/null
            else
                patch -p1 < "$DIR"/Patches/gdb.patch > /dev/null
            fi
            $MD5SUM "$DIR"/Patches/gdb.patch > .patch.applied
        popd
    fi

    if [ -d ${BINUTILS_NAME} ]; then
        rm -rf "${BINUTILS_NAME}"
        rm -rf "$DIR/build/$ARCH/$BINUTILS_NAME"
    fi
    echo "Extracting binutils..."
    tar -xzf ${BINUTILS_PKG}

    pushd ${BINUTILS_NAME}
        if [ "$git_patch" = "1" ]; then
            git init > /dev/null
            git add . > /dev/null
            git commit -am "BASE" > /dev/null
            git apply "$DIR"/Patches/binutils.patch > /dev/null
        else
            patch -p1 < "$DIR"/Patches/binutils.patch > /dev/null
        fi
        $MD5SUM "$DIR"/Patches/binutils.patch > .patch.applied
    popd

    if [ -d ${GCC_NAME} ]; then
        rm -rf "${GCC_NAME}"
        rm -rf "$DIR/build/$ARCH/$GCC_NAME"
    fi
    echo "Extracting gcc..."
    tar -xzf $GCC_PKG
    pushd $GCC_NAME
        if [ "$git_patch" = "1" ]; then
            git init > /dev/null
            git add . > /dev/null
            git commit -am "BASE" > /dev/null
            git apply "$DIR"/Patches/gcc.patch > /dev/null
        else
            patch -p1 < "$DIR/Patches/gcc.patch" > /dev/null
        fi
        $MD5SUM "$DIR/Patches/gcc.patch" > .patch.applied
    popd

    if [ "$SYSTEM_NAME" = "Darwin" ]; then
        pushd "gcc-${GCC_VERSION}"
        ./contrib/download_prerequisites
        popd
    fi
popd


rm -rf "$PREFIX"
mkdir -p "$PREFIX"

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$($NPROC)
fi

mkdir -p "$DIR/build/$ARCH"

pushd "$DIR/build/$ARCH"
    unset PKG_CONFIG_LIBDIR

    if [ "$ARCH" = "aarch64" ]; then
        rm -rf gdb
        mkdir -p gdb

        pushd gdb
            echo "XXX configure gdb"
            buildstep "gdb/configure" "$DIR"/Tarballs/$GDB_NAME/configure --prefix="$PREFIX" \
                                                     --target="$TARGET" \
                                                     --with-sysroot="$SYSROOT" \
                                                     --enable-shared \
                                                     --disable-nls \
                                                     ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1
            echo "XXX build gdb"
            buildstep "gdb/build" "$MAKE" -j "$MAKEJOBS" || exit 1
            buildstep "gdb/install" "$MAKE" install || exit 1
        popd
    fi

    rm -rf binutils
    mkdir -p binutils

    pushd binutils
        echo "XXX configure binutils"
        buildstep "binutils/configure" "$DIR"/Tarballs/$BINUTILS_NAME/configure --prefix="$PREFIX" \
                                                 --target="$TARGET" \
                                                 --with-sysroot="$SYSROOT" \
                                                 --enable-shared \
                                                 --disable-nls \
                                                 ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1
        if [ "$SYSTEM_NAME" = "Darwin" ]; then
            buildstep "binutils/build" "$MAKE" -j "$MAKEJOBS" || true
            pushd intl
            buildstep "binutils/build" "$MAKE" all-yes
            popd
        fi
        echo "XXX build binutils"
        buildstep "binutils/build" "$MAKE" -j "$MAKEJOBS" || exit 1
        buildstep "binutils/install" "$MAKE" install || exit 1
    popd

    echo "XXX pranaos libc, libm and libpthread headers"
    mkdir -p "$BUILD"
    pushd "$BUILD"
        mkdir -p Root/usr/include/
        SRC_ROOT=$($REALPATH "$DIR"/..)
        FILES=$(find "$SRC_ROOT"/kernel/api "$SRC_ROOT"/libs/libc "$SRC_ROOT"/libs/libm "$SRC_ROOT"/libs/libpthread -name '*.h' -print)
        for header in $FILES; do
            target=$(echo "$header" | sed -e "s@$SRC_ROOT/libs/libc@@" -e "s@$SRC_ROOT/libs/libm@@" -e "s@$SRC_ROOT/Userland/Libraries/LibPthread@@" -e "s@$SRC_ROOT/Kernel/@Kernel/@")
            buildstep "system_headers" $INSTALL -D "$header" "Root/usr/include/$target"
        done
        unset SRC_ROOT
    popd

    if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
        perl -pi -e 's/-no-pie/-nopie/g' "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/configure"
    fi

    if [ ! -f "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/pranaos-userland.h" ]; then
        cp "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/pranaos.h" "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/pranaos-kernel.h"
    fi

    rm -rf gcc
    mkdir -p gcc

    pushd gcc
        echo "XXX configure gcc and libgcc"
        buildstep "gcc/configure" "$DIR/Tarballs/gcc-$GCC_VERSION/configure" --prefix="$PREFIX" \
                                            --target="$TARGET" \
                                            --with-sysroot="$SYSROOT" \
                                            --disable-nls \
                                            --with-newlib \
                                            --enable-shared \
                                            --enable-languages=c,c++ \
                                            --enable-default-pie \
                                            --enable-lto \
                                            --enable-threads=posix \
                                            ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1

        echo "XXX build gcc and libgcc"
        buildstep "gcc/build" "$MAKE" -j "$MAKEJOBS" all-gcc || exit 1
        if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
            ln -sf liblto_plugin.so.0.0 gcc/liblto_plugin.so
        fi
        buildstep "libgcc/build" "$MAKE" -j "$MAKEJOBS" all-target-libgcc || exit 1
        echo "XXX install gcc and libgcc"
        buildstep "gcc+libgcc/install" "$MAKE" install-gcc install-target-libgcc || exit 1

        echo "XXX build libstdc++"
        buildstep "libstdc++/build" "$MAKE" -j "$MAKEJOBS" all-target-libstdc++-v3 || exit 1
        echo "XXX install libstdc++"
        buildstep "libstdc++/install" "$MAKE" install-target-libstdc++-v3 || exit 1
    popd

    if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
        cd "$DIR/Local/${ARCH}/libexec/gcc/$TARGET/$GCC_VERSION" && ln -sf liblto_plugin.so.0.0 liblto_plugin.so
    fi
popd


pushd "$DIR"
    if [ "${TRY_USE_LOCAL_TOOLCHAIN}" = "y" ] ; then
        echo "::endgroup::"
        echo "Building cache tar:"

        rm -f "${CACHED_TOOLCHAIN_ARCHIVE}"  

        tar czf "${CACHED_TOOLCHAIN_ARCHIVE}" Local/

        echo "Cache (after):"
        ls -l Cache
    fi
popd