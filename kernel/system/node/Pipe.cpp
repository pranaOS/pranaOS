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
    return !_buffer.empty() || !writers();
}

bool FsPipe::can_write(FsHandle &)
{
    return !_buffer.full() || !readers();
}

ResultOr<size_t> FsPipe::read(FsHandle &handle, void *buffer, size_t size)
{
    UNUSED(handle);

    if (!writers() && _buffer.empty())
    {
        return ERR_STREAM_CLOSED;
    }

    return _buffer.read((char *)buffer, size);
}

ResultOr<size_t> FsPipe::write(FsHandle &handle, const void *buffer, size_t size)
{
    UNUSED(handle);

    if (!readers())
    {
        return ERR_STREAM_CLOSED;
    }

    return _buffer.write((const char *)buffer, size);
}