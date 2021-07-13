/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include <unistd.h>

#include <abi/Syscalls.h>

ssize_t read(int fd, void *buf, size_t count)
{
    size_t read = 0;
    JResult result = J_handle_read(fd, buf, count, &read);

    if (result != JResult::SUCCESS)
    {
        return -1;
    }
    else
    {
        return (ssize_t)read;
    }
}

ssize_t write(int fd, const void *buf, size_t count)
{
    size_t written = 0;
    JResult result = J_handle_write(fd, buf, count, &written);

    if (result != JResult::SUCCESS)
    {
        return -1;
    }
    else
    {
        return (ssize_t)written;
    }
}

int unlink(const char *pathname)
{
    return J_filesystem_unlink(pathname, strlen(pathname)) == JResult::SUCCESS ? 0 : -1;
}

int rmdir(const char *dirname)
{
    return unlink(dirname);
}

int close(int fd)
{
    return J_handle_close(fd) == JResult::SUCCESS ? 0 : -1;
}

off_t lseek(int fd, off_t offset, int whence)
{
    ssize64_t offset_64 = offset;
    ssize64_t offset_beg;

    JResult result = J_handle_seek(fd, &offset_64, (JWhence)whence, &offset_beg);
    if (result != JResult::SUCCESS)
    {
        // TODO: set errno
        return (off_t)-1;
    }

    return offset_beg;
}