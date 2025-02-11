/**
 * @file kmalloc.cpp
 * @author Krisna Pranav
 * @brief kmalloc
 * @version 6.0
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#if defined(__pranaos__) && !defined(KERNEL)

#include <mods/assertions.h>
#include <mods/kmalloc.h>

/**
 * @param size 
 * @return void* 
 */
void* operator new(size_t size)
{
    void* ptr = malloc(size);
    VERIFY(ptr);
    return ptr;
}

/**
 * @param size 
 * @return void* 
 */
void* operator new(size_t size, std::nothrow_t const&) noexcept
{
    return malloc(size);
}

/**
 * @param size 
 * @return void* 
 */
void* operator new[](size_t size)
{
    void* ptr = malloc(size);
    VERIFY(ptr);
    return ptr;
}

/**
 * @param size 
 * @return void* 
 */
void* operator new[](size_t size, std::nothrow_t const&) noexcept
{
    return malloc(size);
}

/**
 * @param ptr 
 */
void operator delete[](void* ptr) noexcept
{
    return free(ptr);
}

/**
 * @param ptr 
 */
void operator delete[](void* ptr, size_t) noexcept
{
    return free(ptr);
}

#endif