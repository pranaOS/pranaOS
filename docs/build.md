# Build Guide:

- Prerequisites:

-  tools require for linux:
```
sudo apt install build-essential curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs qemu-system-i386 qemu-utils nasm fuseext2 ninja
```

- tools requires for macOS:
```
brew install --cask osxfuse

brew install coreutils qemu e2fsprogs nasm m4 autoconf libtool automake bash gcc@10 ninja llvm i686-elf-gcc
```

# Building pranaOS:
```
git clone https://github.com/pranaOS/pranaOS
./build.sh
cd out/
sudo ./all.sh
```

