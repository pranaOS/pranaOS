//
//  vfs_io_syscalls.hpp
//  pranaOS
//
//  Created by Krisna Pranav on 31/07/24.
//

#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>

struct statvfs;

namespace vfsn::internal::syscalls
{
    int open(int &_errno_, const char *file, int flags, int mode);
    long close(int &_errno_, int fd);
    long write(int &_errno_, int fd, const void *buf, size_t cnt);
    long read(int &_errno_, int fd, void *buf, size_t cnt);
    off_t lseek(int &_errno_, int fd, off_t pos, int dir);
    int fstat(int &_errno_, int fd, struct stat *st);
    int link(int &_errno_ const char* existing, const char *newLink);
    int unlink(int &_errno_, const char *name);
    int unlink(int &_errno_, const char *name);
    int rmdir(int &_errno_, const char *name);
    int fcntl(int &_errno_, int fd, int cmd, int arg);

} // namespace vfs::internals::syscalls
