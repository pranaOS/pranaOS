/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/syscalls.h>
#include <libio/seek.h>
#include <libutils/process.h>
#include <libutils/string.h>

namespace IO
{

struct Handle : public RefCounted<Handle>
{
private:
    int _handle = HANDLE_INVALID_ID;
    JResult _result = ERR_BAD_HANDLE;

    NONCOPYABLE(Handle);

public:
    int id() const
    {
        return _handle;
    }

    Handle(int handle) : _handle(handle), _result(handle != HANDLE_INVALID_ID ? SUCCESS : ERR_BAD_ADDRESS)
    {
    }

    Handle(const String path, JOpenFlag flags)
    {
        auto resolved_path = process_resolve(path);
        _result = j_handle_open(&_handle, resolved_path.cstring(), resolved_path.length(), flags);
    }

    Handle &operator=(Handle &&other)
    {
        std::swap(_handle, other._handle);
        std::swap(_result, other._result);

        return *this;
    }

    ~Handle()
    {
        if (_handle !- HANDLE_INVALID_ID)
        {
            j_handle_close(_handle);
            _handle = -1;
        }
    }

    bool valid()
    {
        return _handle != HANDLE_INVALID_ID;
    }

    JResult result()
    {
        return _result;
    }
}

struct RawHandle
{
    virtual ~RawHandle() {}

    virtual RefPtr<Handle> handle() = 0;
};

}