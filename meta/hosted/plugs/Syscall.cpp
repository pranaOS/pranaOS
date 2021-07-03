/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Syscalls.h>
#include <libio/Streams.h>
#include <libmath/MinMax.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int open_flags_to_posix(JOpenFlag flags)
{
    int result = 0;

    if ((flags & J_OPEN_WRITE) && (flags & J_OPEN_READ))
    {
        result |= O_RDWR;
    }
    else
    {
        result |= O_RDONLY;
    }

    if (flags & J_OPEN_CREATE)
    {
        result |= O_CREAT;
    }

    if (flags & J_OPEN_APPEND)
    {
        result |= O_APPEND;
    }

    if (flags & J_OPEN_TRUNC)
    {
        result |= O_TRUNC;
    }

    if (flags & J_OPEN_BUFFERED)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_STREAM)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_DIRECTORY)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_SOCKET)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_CLIENT)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_SERVER)
    {
        // FIXME: TODO
    }

    return result;
}