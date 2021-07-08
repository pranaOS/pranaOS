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

    // Open Mode
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

    // Flags
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