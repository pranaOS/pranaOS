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



};

}