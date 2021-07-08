/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fcntl.h>
#include <string.h>
#include <libabi/Syscalls.h>

JOpenFlag fcntl_parse_mode(int mode)
{
    JOpenFlag flags = J_OPEN_STREAM;

    if (mode == O_RDONLY)
    {
        flags |= J_OPEN_READ;
    }
    else if (mode == O_WRONLY)
    {
        flags |= J_OPEN_WRITE;
    }
    else if (mode == O_RDWR)
    {
        flags |= J_OPEN_READ | J_OPEN_WRITE;
    }

    if (mode & O_CREAT)
    {
        flags |= J_OPEN_CREATE;
    }
    if (mode & O_APPEND)
    {
        flags |= J_OPEN_APPEND;
    }
    if (mode & O_DIRECTORY)
    {
        flags |= J_OPEN_DIRECTORY;
    }

    return flags;
}

int open(const char *path, int mode, ...)
{
    int flags = fcntl_parse_mode(mode);
    int handle = 0;
    JResult result = J_handle_open(&handle, path, strlen(path), flags);

    if (result != JResult::SUCCESS)
    {
        return -1;
    }

    return handle;
}