/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <abi/Syscalls.h>
#include <assert.h>
#include <libsystem/core/Plugs.h>

Tick __plug_system_get_ticks()
{
    Tick result = 0;
    assert(J_system_tick(&result) == SUCCESS);
    return result;
}