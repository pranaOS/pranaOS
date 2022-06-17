FROM debian

USER root

RUN apt-get update && \
    apt-get install -y sudo python nasm build-essential wget libmpfr-dev libmpc-dev libgmp3-dev \
            texinfo m4 kpartx grub2-common grub-pc-bin lbzip2 xutils-dev help2man autoconf automake \
            git gperf ninja-build libwayland-bin pkg-config libexpat-dev bison flex python3-pip \
            autopoint gettext python3-mako cmake zlib1g-dev libssl-dev doxygen

RUN pip3 install meson
            