# Building pranaOS:

- tools need:

- linux:
```
sudo apt install build-essential curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs qemu-system-i386 qemu-utils nasm fuseext2 ninja
```


- macos:
```
brew install --cask osxfuse

brew install coreutils qemu e2fsprogs nasm m4 autoconf libtool automake bash gcc@10 ninja
```

# Building:
```
git clone https://github.com/pranaOS/pranaOS
./build.sh
./out/all.sh
```