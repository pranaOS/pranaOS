/**
 * @file memory.h
 * @author Krisna Pranav
 * @brief memory
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>

#if defined(KERNEL)
#   include <kernel/stdlib.h>
#else
#   include <stdlib.h>
#   include <string.h>
#endif

ALWAYS_INLINE void fast_u32_copy(u32* dest, const u32* src, size_t count)
{
    
}