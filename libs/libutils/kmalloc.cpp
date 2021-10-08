/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#if defined(__pranaOS__) && !defined(KERNEL)

#   include <libutils/assertions.h>
#   include <libutils/kmalloc.h>

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

void operator delete(void* ptr) noexcept
{
    return free(ptr);
}

void* operator new[](size_t size)
{
    void* ptr = malloc(size);
    return ptr;
}


#endif