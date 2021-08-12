# Building pranaOS:

- building pranaOS will take upto 1 hr to 2 hrs it depends on your cpu

# Linux / Debian:
```
sudo apt install build-essential cmake curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs ninja-build qemu-system-i386 qemu-utils ccache rsync libpixman-1-dev libgtk-3-dev
```

# macOS:
```bash
brew install coreutils e2fsprogs qemu bash gcc@10 imagemagick ninja cmake ccache rsync

# option 1
brew install m4 autoconf automake libtool
brew install --cask osxfuse
toolchain/buildfuseext2.sh

# option 2
brew install genext2fs (i recommend this)
```

# Arch Linux:
```
sudo pacman -S --needed base-devel cmake curl mpfr libmpc gmp e2fsprogs ninja qemu qemu-arch-extra ccache rsync
```
