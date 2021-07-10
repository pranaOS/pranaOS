/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libabi/Syscalls.h>
#include <libabi/Result.h>
#include <libsystem/system/Memory.h>

JResult memory_alloc(size_t size, uintptr_t *out_address)
{
    return J_memory_alloc(size, out_address);
}

JResult memory_free(uintptr_t address)
{
    return J_memory_free(address);
}

JResult memory_include(int handle, uintptr_t *out_address, size_t *out_size)
{
    return J_memory_include(handle, out_address, out_size);
}

JResult memory_get_handle(uintptr_t address, int *out_handle)
{
    return J_memory_get_handle(address, out_handle);
}