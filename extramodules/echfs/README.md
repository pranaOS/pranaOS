# echfs

The echfs filesystem is a 64-bit FAT-like filesystem which aims to support
most UNIX and POSIX-style features while being extremely simple to implement.
Ideal for hobbyist OS developers who want a simple filesystem and don't want
to deal with old crufty FAT (which isn't UNIX/POSIX compliant either),
or complex filesystems such as ext2/3/4.

Keep in mind that this is still a work in progress, and the specification might change.
I'll try to keep everything backwards compatible (in a clean way)
when I add new features or make modifications to the filesystem.

In this repo you can find the full specification in the `spec.txt` file,
and a utility to manipulate the filesystem (`echfs-utils`).
You can compile and install the `echfs-utils` program using `make` the usual way.

A FUSE implementation of a filesystem driver named `echfs-fuse` is also provided (thanks to Geertiebear).

# Build dependencies

`echfs-fuse` depends on `libuuid`, `libfuse`, and `pkg-config`. (On Debian/Ubuntu based distros,
the packages are called `uuid-dev`, `libfuse-dev`, and `pkg-config`, respectively).

On systems where FUSE is not available, it is possible to compile `echfs-utils`
exclusively by running `make utils` instead of `make` and `make install-utils`
instead of `make install`.

# Building

```
make
sudo make install
```

# Usage

## echfs-utils

echfs-utils is used as ``echfs-utils <flags> <image> <command> <command args...>``, where
a command can be any of the following:

* ``import``, which copies to the image with args ``<source> <destination>``
* ``export``, which copies from the image  with args ``<source> <destination>``
* ``ls``, with arg ``<path>`` (can be left empty), it lists the files in the path or
 root if the path is not specified
* ``mkdir``, with arg ``<path>``, makes a directory with the specified path.
* ``format``, with arg ``<block size>`` formats the image
* ``quick-format`` with arg ``<block size>`` formats the image

There are also several flags you can specify

* ``-f`` ignore existing file errors on ``import``
* ``-m`` specify that the image is MBR formatted
* ``-g`` specify that the image is GPT formatted
* ``-p <part>`` specify which partition the echfs image is in
* ``-v`` be verbose

## echfs-fuse

echfs-fuse is used as ``echfs-fuse <flags> <image> <mountpoint>``, with the following flags:

* ``-m`` specify that the image is MBR formatted
* ``-g`` specify that the image is GPT formatted
* ``-p <part>`` specify which partition the echfs image is in
* ``-d`` run in debug mode (don't detach)

## Creating a filesystem

A filesystem can be created with the following commands
```
dd if=/dev/zero of=image.hdd bs=4M count=128
parted -s image.hdd mklabel msdos
parted -s image.hdd mkpart primary 2048s 100%
echfs-utils -m -p0 image.hdd quick-format 512
```
