/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/Syscalls.h>
#include <libio/Seek.h>
#include <libsystem/process/Process.h>
#include <libutils/String.h>

namespace IO
{

struct Handle :
    public RefCounted<Handle>
{

private:
    int _handle = HANDLE_INVALID_ID;
    JResult _result = ERR_BAD_HANDLE;

    NONCOPYABLE(Handle);

public:
    public:
    int id() const { return _handle; }

    Handle(int handle) : _handle(handle), _result(handle != HANDLE_INVALID_ID ? SUCCESS : ERR_BAD_HANDLE)
    {
    }

    Handle(const String path, JOpenFlag flags)
    {
        auto resolved_path = process_resolve(path);
        _result = J_handle_open(&_handle, resolved_path.cstring(), resolved_path.length(), flags);
    }

    Handle(Handle &&other)
    {
        _handle = std::exchange(other._handle, HANDLE_INVALID_ID);
        _result = std::exchange(other._result, ERR_BAD_HANDLE);
    }

    Handle &operator=(Handle &&other)
    {
        std::swap(_handle, other._handle);
        std::swap(_result, other._result);
        
        return *this;
    }

    ~Handle()
    {
        if (_handle != HANDLE_INVALID_ID)
        {
            J_handle_close(_handle);
            _handle = -1;
        }
    }

    ResultOr<size_t> read(void *buffer, size_t size)
    {
        size_t data_read = 0;
        _result = TRY(J_handle_read(_handle, buffer, size, &data_read));
        return data_read;
    }


};

}