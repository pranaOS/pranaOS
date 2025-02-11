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
#include <kernel/stdlib.h>
#else
#include <stdlib.h>
#include <string.h>
#endif

/**
 * @param dest 
 * @param src 
 * @param count 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE void fast_u32_copy(u32* dest, u32 const* src, size_t count)
{
#if ARCH(I386) || ARCH(X86_64)
    asm volatile(
        "rep movsl\n"
        : "+S"(src), "+D"(dest), "+c"(count)::"memory");
#else
    __builtin_memcpy(dest, src, count * 4);
#endif
}

/**
 * @param dest 
 * @param value 
 * @param count 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE void fast_u32_fill(u32* dest, u32 value, size_t count)
{
#if ARCH(I386) || ARCH(X86_64)
    asm volatile(
        "rep stosl\n"
        : "=D"(dest), "=c"(count)
        : "D"(dest), "c"(count), "a"(value)
        : "memory");
#else
    for(auto* p = dest; p < (dest + count); ++p)
    {
        *p = value;
    }
#endif
}

namespace Mods
{
    /**
     * @param ptr 
     * @param size 
     */
    inline void secure_zero(void* ptr, size_t size)
    {
        __builtin_memset(ptr, 0, size);
        
        asm volatile("" ::
                        : "memory");
    }

    /**
     * @param b1 
     * @param b2 
     * @param len 
     * @return true 
     * @return false 
     */
    inline bool timing_safe_compare(void const* b1, void const* b2, size_t len)
    {
        auto* c1 = static_cast<char const*>(b1);
        auto* c2 = static_cast<char const*>(b2);

        u8 res = 0;
        
        for(size_t i = 0; i < len; i++)
        {
            res |= c1[i] ^ c2[i];
        }

        return !res;
    }

} // namespace Mods

using Mods::secure_zero;
using Mods::timing_safe_compare;
