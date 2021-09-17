/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libio/handle.h>
#include <libio/path.h>
#include <libio/reader.h>
#include <libio/writer.h>

namespace IO
{

struct Connection final : public Reader, public Writer, public RawHandle
{
private:
    RefPtr<Handle> _handle;

public:
    RefPtr<Handle> handle() override
    {
        return _handle;
    }

    Connection() 
    {
    }

    Connection(RefPtr<Handle> handle) : _handle{handle} {}

    ResultOr<size_t> read(void *buffer, size_t size)
    {
        if (!handle)
            return ERR_STREAM_CLOSED;
        
        return _handle->read(buffer, size);
    }

    bool closed()
    {
        return _handle == nullptr;
    }

    void close()
    {
        _handle = nullptr;
    }
};

}