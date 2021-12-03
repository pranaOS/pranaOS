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
    
    def getattr(self, path, fh=None):
        (kind, addr, size, time, name) = self.__scan(path)
        if addr == 0:
            raise FuseOSError(ENOENT)
        mode = S_IFDIR | 0o755 if kind == 0 else S_IFREG | 0o644
        return { "st_atime": 0, "st_mtime": time, "st_uid": 0, "st_gid": 0, "st_mode": mode, "st_size": size }
    
if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('img')
    parser.add_argument('mount')
    args = parser.parse_args()
    fuse = FUSE(pranaOSFuse(args.img), args.mount, ro=True, foreground=True, allow_other=True)
