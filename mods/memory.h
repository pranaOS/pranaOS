/**
 * @file memory.h
 * @author Krisna Pranav
 * @brief memory
 * @version 6.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>

#if defined(KERNEL)
#   include <kernel/stdlib.h>
#else
#    include <stdlib.h>
#    include <string.h>
#endif

/**
 * @param dest 
 * @param src 
 * @param count 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE void fast_u32_copy(u32* dest, const u32* src, size_t count)
{
    asm volatile(
        "rep movsl\n"
        : "+S"(src), "+D"(dest), "+c"(count)::"memory");
}

/**
 * @param dest 
 * @param value 
 * @param count 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE void fast_u32_fill(u32* dest, u32 value, size_t count)
{
    asm volatile(
        "rep stosl\n"
        : "=D"(dest), "=c"(count)
        : "D"(dest), "c"(count), "a"(value)
        : "memory");
}
