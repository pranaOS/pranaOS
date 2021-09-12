/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/prelude.h>

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

typedef unsigned int JOpenFlag;

struct JState
{
    size_t size;
    JFileType type;
};

struct JDirEntry
{
    char name[FILE_NAME_LENGTH];
    JStat stat;
};