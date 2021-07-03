/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once
#include <libabi/Syscalls.h>
#include <assert.h>
#include <libutils/SourceLocation.h>

#ifdef __KERNEL__
#    include "system/interrupts/Interupts.h"
#endif

namespace Utils
{

struct Lock
{
private:
    static constexpr auto NO_HOLDER = 0xDEADDEAD;
    
    bool _locked = false;
    int _holder = NO_HOLDER;
    const char *_name = "lock-not-initialized";

    SourceLocation _last_acquire_location{};
    SourceLocation _last_release_location{};

    NONMOVABLE(Lock);
    NONCOPYABLE(Lock);

    void ensure_failed(const char *raison, SourceLocation location)
    {
        UNUSED(raison);
        UNUSED(location);
        ASSERT_NOT_REACHED();
    }

};
}