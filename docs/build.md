# Build Guide

# Supported Distros:

- debian
- arch
- ubuntu
- mac
- fedora & redhat

# Installing Requirements:

- Debian, Ubuntu
```bash
$ sudo apt install build-essential curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs qemu-system-i386 qemu-utils nasm fuseext2 ninja wget

# llvm installation
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 12
```

- Macos:
```bash
brew install --cask osxfuse

brew install coreutils qemu e2fsprogs nasm m4 autoconf libtool automake bash gcc@10 ninja llvm
```

- Gnu Toolchain for MacOS:

x86:
```
brew install i686-elf-gcc
```

Aarch32:
```
brew tap pranaOS/homebrew-formulae-arm-gcc
brew install pranaOS/homebrew-formulae-arm-gcc
```

- Toolchain for Linux:

x86
```
./toolchains/scripts/i686-elf-tools.sh
```

Aarch32
```
./toolchains/scripts/arm-none-eabi-tools.sh
```


# Building pranaOS

- Generate Ninja Image

```./gn_gen.sh``` This command creates a build directory called ```out/``` and disk image will be there



#### **Options of ./gn.gen**

* --target_cpu *value*
  * Sets target arch
  * Possible values:
    * x86 *(default)*
    * aarch32
* --host *value*
  * Sets toolchain to build the OS
  * Possible values:
    * gnu *(default)*
    * llvm
* --device_type *value*
  * Configueres OS parts to work in either desktop or mobile mode.
  * Possible values:
    * d / desktop *(default)*
    * m / mobile
* --bench *value*
  * Possible values:
    * none *(default)*
    * external_script

So to build pranaOS for Arm with LLVM you have to generate Ninja files with `./gn_gen.sh --target_cpu aarch32 --host llvm`

#### **Environment variables**

Another option how to configure the project is environment variables.

* `PRANAOS_QEMU_SMP`
  * Provide cpu cores count in the system.
* `PRANAOS_QEMU_X86`
  * Provide path to *qemu-system-i386* executable
* `PRANAOS_QEMU_ARM`
  * Provide path to *qemu-system-arm* executable
* `LLVM_BIN_PATH`  *(Only with --host llvm)*
  * ***Must be set before `./gn_gen.sh`***
  * Provide path to LLVM bins.

</br>

### Building

Move to `out/` directory where the OS will be built. There are several scripts:

* `build.sh` - builds OS
* `sync.sh` - synchronise files with the disk image
* `run.sh` - launches QEMU

The right sequence to run the OS is to build, sync, launch or use `all.sh` which combine these 3 scripts.

### Debugging

There are several scripts which might help you to debug the OS:

* `debug.sh` - launches QEMU in debug mode. Debug analog of `run.sh`
* `dll.sh` - Debug analog of `all.sh`.