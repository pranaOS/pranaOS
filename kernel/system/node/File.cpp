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