/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#if defined(__pranaos__) && !defined(KERNEL)

#    include <base/Assertions.h>
#    include <base/kmalloc.h>


void* operator new(size_t size)
{
    void* ptr = malloc(size);
    VERIFY(ptr);
    return ptr;
}

void* operator new(size_t size, const std::nothrow_t&) noexcept
{
    return malloc(size);
}

void operator new[](size_t size, const std::nothrow_t&) noexcept
{
    return malloc(size);
}

void operator delete[](void* ptr, size_t) noexcept
{
    return free(ptr);
}

#endif