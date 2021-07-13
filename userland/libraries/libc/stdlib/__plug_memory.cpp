/*
 * Copyright (c) 2021, Krisna Pranav
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

void *__plug_memory_alloc(size_t size)
{
    uintptr_t address = 0;
    assert(j_memory_alloc(size, &address) == JResult::SUCCESS);
    return (void *)address;
}

void __plug_memory_free(void *address, size_t size)
{
    UNUSED(size);
    j_memory_free((uintptr_t)address);
}