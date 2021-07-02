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
$ sudo apt install nasm gcc make binutils grub-pc-bin xorriso mtools git qemu qemu-kvm build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
```

- Arch
```
$ sudo pacman -S nasm gcc make binutils grub qemu libisoburn mtools git qemu base-devel bison flex mpc mpfr texinfo
```

- Fedora, Redhat
```
$ sudo dnf install nasm gcc gcc-g++ make binutils xorriso mtools ImageMagick git qemu qemu-kvm patch bison flex mpc-devel mpfr-devel gmp-devel texinfo patch 
```

# Building pranaOS

- clone the repo:

```
$ git clone https://github.com/pranaOS/pranaOS
$ cd pranaOS
```

- then for building pranaos follow these steps:

```
$ toolchain/buildit.sh
```
- building will take upto 1 - 2 hrs it depends on your processor & ram


# Building Image and running

```
$ cd ../ && make all
```

- running pranaOS
```sh
# for qemu
$ make run VMACHINE=qemu
# for virtualbox
$ make run VMACHINE=vbox
```

- if you like this project give a star & fork it 🙂