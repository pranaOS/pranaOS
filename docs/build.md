# Building pranaOS:

- tools need:

- linux:
```
sudo apt install build-essential curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs qemu qemu-utils nasm fuseext2 ninja
```

- macos:
```
brew install --cask osxfuse

brew install coreutils qemu e2fsprogs nasm m4 autoconf libtool automake bash gcc@10 ninja llvm i686-elf-gcc
```

# gn:
- you need to download gn
- what is gn?:
```
it is a compiler built by google
```
- get it from [here](https://gn.googlesource.com/gn/)
- it is very important for this project


# Building pranaOS for x86:
```
git clone https://github.com/pranaOS/pranaOS
./build.sh
./out/all.sh
```

# Building pranaOS for arm:

- arm compiler
```
brew tap pranaOS/homebrew-formulae-arm-gcc
brew install pranaOS/homebrew-formulae-arm-gcc/arm-none-eabi-gcc
```

- building:
```
./build.sh --target_cpu aarch32
cd out
./all.sh
```