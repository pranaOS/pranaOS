# Build Guide

# Supported Distros:

- debian
- arch
- ubuntu
- mac
- fedora & redhat

# Installing Requirements:

- Debian, Ubuntu
```
$ sudo apt install build-essential cmake curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs ninja-build qemu-system-i386 qemu-utils ccache rsync
$ sudo apt install gcc-10 g++-10
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 900 --slave /usr/bin/g++ g++ /usr/bin/g++-10
$ sudo apt install libpixman-1-dev libgtk-3-dev
```

- Macos:
```bash
brew install coreutils qemu bash gcc@10 ninja cmake ccache rsync

# (option 1) fuse + ext2
brew install e2fsprogs m4 autoconf automake libtool
brew install --cask osxfuse
toolchain/BuildFuseExt2.sh

# (option 2) genext2fs
brew install genext2fs
```

- Arch
```
$ sudo pacman -S --needed base-devel cmake curl mpfr libmpc gmp e2fsprogs ninja qemu qemu-arch-extra ccache rsync
```

- Fedora
```
$ sudo dnf install binutils-devel curl cmake mpfr-devel libmpc-devel gmp-devel e2fsprogs ninja-build patch ccache rsync @"C Development Tools and Libraries" @Virtualization
```

- openSUSE
```
$ sudo zypper install curl cmake mpfr-devel mpc-devel ninja gmp-devel e2fsprogs patch qemu-x86 qemu-audio-pa gcc gcc-c++ ccache rsync patterns-devel-C-C++-devel_C_C++
```

# Building pranaOS

- clone this repo
```
git clone https://github.com/pranaOS/pranaOS
```

- build pranaOS
```
Meta/pranaos.sh build-toolchain
Meta/pranaos.sh run
```
