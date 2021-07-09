/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include <unistd.h>
#include <libabi/Syscalls.h>

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

ssize_t write(int fd, void *buf, size_t count)
{
    size_t read = 0;
    JResult result = J_handle_read(fd, buf, count, &written);

    if (result != JResult::SUCCESS)
    {
        return -1;
    }
    else
    {
        return (ssize_t)written;
    }
}

