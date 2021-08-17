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

# gn:
- you need to download gn 
- what is gn?:
```
it is a compiler built by google
```
- get it from [here](https://github.com/pranaOS/pranaOS/blob/master/gn/gn?raw=true)
- it is very important for this project


# Building:
```
git clone https://github.com/pranaOS/pranaOS
./build.sh
./out/all.sh
```