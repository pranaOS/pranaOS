/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libutils/Prelude.h>

#define FILE_NAME_LENGTH (64)
#define PATH_LENGTH (512)
#define PATH_DEPTH (16)
#define PATH_SEPARATOR '/'

enum JWhence
{
    J_WHENCE_START,
    J_WHENCE_CURRENT,
    J_WHENCE_END,
};

enum JFileType
{
    J_FILE_TYPE_UNKNOWN,

    J_FILE_TYPE_REGULAR,
    J_FILE_TYPE_DEVICE,
    J_FILE_TYPE_DIRECTORY,
    J_FILE_TYPE_PIPE,
    J_FILE_TYPE_SOCKET,
    J_FILE_TYPE_CONNECTION,
    J_FILE_TYPE_TERMINAL,
};

#define J_OPEN_READ (1 << 0)
#define J_OPEN_WRITE (1 << 1)
#define J_OPEN_CREATE (1 << 2)
#define J_OPEN_APPEND (1 << 3)
#define J_OPEN_TRUNC (1 << 4)
#define J_OPEN_BUFFERED (1 << 5)
#define J_OPEN_STREAM (1 << 6)
#define J_OPEN_DIRECTORY (1 << 7)
#define J_OPEN_SOCKET (1 << 8)
#define J_OPEN_CLIENT (1 << 9)
#define J_OPEN_SERVER (1 << 10)

typedef unsigned int JOpenFlag;

struct JStat
{
    size_t size;
    JFileType type;
};

struct JDirEntry
{
    char name[FILE_NAME_LENGTH];
    JStat stat;
};