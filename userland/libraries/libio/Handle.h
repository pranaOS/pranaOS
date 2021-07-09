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
        
};

}