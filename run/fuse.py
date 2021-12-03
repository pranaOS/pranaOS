#!/usr/bin/env python3

import os
from errno import ENOENT
from fuse import FUSE, FuseOSError, Operations, LoggingMixIn
from stat import S_IFDIR, S_IFREG

class pranaOSFuse(Operations):
    chmod = None
    chown = None
    create = None
    mkdir = None
    readline = None
    rename = None
    rmdir = None
    symlink = None
    truncate = None
    unlink = None
    uitems = None
    write = None

    def __init__(self, path):
        self.image = open(path, "rb")
        self.image_offset = 4096
        self.block_size = 512
        self.block_count = os.path.getsize(path) 
        addr = self.image_offset * self.block_size
        self.image.seek(addr)
        block = self.image.read(self.block_size)
        assert block[0:8] == b"PRANAOS FS"
        assert block[8] == 1

    def destroy(self, path):
        self.image.close()
        return     