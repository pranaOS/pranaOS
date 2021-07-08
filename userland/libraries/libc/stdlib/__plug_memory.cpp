/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <libabi/Syscalls.h>
#include <libutils/Lock.h>
#include <pranaos/Plugs.h>

static Lock _memory_lock{"memory_lock"};

void __plug_memory_lock()
{
    _memory_lock.acquire();
}

void __plug_memory_unlock()
{
    _memory_lock.release();
}