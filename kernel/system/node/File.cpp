/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libabi/Result.h>
#include <libmath/MinMax.h>
#include <string.h>
#include "system/node/File.h"
#include "system/node/Handle.h"


FsFile::FsFile() : FsNode(J_FILE_TYPE_REGULAR)
{
    _buffer = (char *)malloc(512);
    _buffer_allocated = 512;
    _buffer_size = 0;
}

FsFile::~FsFile()
{
    free(_buffer);
}

JResult FsFile::open(FsHandle &handle)
{
    if (handle.has_flag(J_OPEN_TRUNC))
    {
        free(_buffer);
        _buffer = (char *)malloc(512);
        _buffer_allocated = 512;
        _buffer_size = 0;
    }

    return SUCCESS;
}

size_t FsFile::size()
{
    return _buffer_size;
}

