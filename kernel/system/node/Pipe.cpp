/*
 * Copyright (c) 2021, krishpranav, Alex5xt
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Result.h>
#include "system/node/Handle.h"
#include "system/node/Pipe.h"

FsPipe::FsPipe() : FsNode(J_FILE_TYPE_PIPE)
{
}

bool FsPipe::can_read(FsHandle &)
{
    return !_buffer.empty() || !wirtter();
}

bool FsPipe::can_write(FsHandle &)
{
    return !_buffer.full() || !readers();
}
