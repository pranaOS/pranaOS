#!/bin/bash

# install dependencies required for this project
echo "INSTALLING DEPENDENCIES"
sleep 1
sudo apt-get update
sudo apt install -y nasm gcc make binutils grub-pc-bin qemu-system-x86
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev
sudo apt install -y xorriso mtools libmpfr-dev texinfo libfuse-dev
sudo apt install -y gcc-multilib clang-tidy uuid-dev
