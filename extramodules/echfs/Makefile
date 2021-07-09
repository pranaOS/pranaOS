CC=cc
OBJCOPY=objcopy
DESTDIR=
PREFIX=/usr/local
CFLAGS=-O3 -Wall -Wextra -pipe

.PHONY: all clean install-fuse install-utils install-mkfs install

all: echfs-utils echfs-fuse mkfs.echfs

boot.bin: boot.asm
	nasm -fbin -o boot.bin boot.asm

boot.o: boot.bin
	$(OBJCOPY) -B i8086 -I binary -O default boot.bin boot.o

echfs-utils: echfs-utils.c part.c part.h
	$(CC) $(CFLAGS) part.c echfs-utils.c -luuid -o echfs-utils

echfs-fuse: echfs-fuse.c part.c part.h
	$(CC) $(CFLAGS) part.c echfs-fuse.c $(shell pkg-config fuse --cflags --libs) -o echfs-fuse

mkfs.echfs: boot.o mkfs.echfs.c
	$(CC) $(CFLAGS) boot.o mkfs.echfs.c -o mkfs.echfs

clean:
	rm -f echfs-utils
	rm -f echfs-fuse
	rm -f mkfs.echfs
	rm -f boot.bin boot.o

install-mkfs: mkfs.echfs
	install -d $(DESTDIR)$(PREFIX)/bin
	install -s mkfs.echfs $(DESTDIR)$(PREFIX)/bin

install-utils: echfs-utils
	install -d $(DESTDIR)$(PREFIX)/bin
	install -s echfs-utils $(DESTDIR)$(PREFIX)/bin

install-fuse: echfs-fuse
	install -d $(DESTDIR)$(PREFIX)/bin
	install -s echfs-fuse $(DESTDIR)$(PREFIX)/bin
	ln -sf $(DESTDIR)$(PREFIX)/bin/echfs-fuse $(DESTDIR)$(PREFIX)/bin/mount.echfs-fuse
	ln -sf $(DESTDIR)$(PREFIX)/bin/echfs-fuse $(DESTDIR)$(PREFIX)/bin/mount.echfs

install: install-utils install-fuse install-mkfs
