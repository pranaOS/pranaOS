/**
 * @file random.h
 * @author Krisna Pranav
 * @brief Random
 * @version 6.0
 * @date 2023-07-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/platform.h>
#include <mods/types.h>

#if defined(__pranaos__)
#include <stdlib.h>
#endif

#if defined(__unix__)
#include <unistd.h>
#endif

#if defined(MODS_OS_MACOS)
#include <sys/random.h>
#endif

namespace Mods
{
    /**
     * @param buffer 
     * @param length 
     */
    inline void fill_with_random([[maybe_unused]] void* buffer, [[maybe_unused]] size_t length)
    {
    #if defined(__pranaos__)
        arc4random_buf(buffer, length);
    #elif defined(OSS_FUZZ)
    #elif defined(__unix__) or defined(AK_OS_MACOS)
        [[maybe_unused]] int rc = getentropy(buffer, length);
    #endif
    }

    /**
     * @brief Get the random object
     * 
     * @tparam T 
     * @return T 
     */
    template <typename T>
    inline T get_random()
    {
        T t;
        fill_with_random(&t, sizeof(T));
        return t;
    }

    /**
     * @brief Get the random uniform object
     * 
     * @param max_bounds 
     * @return u32 
     */
    u32 get_random_uniform(u32 max_bounds);

} // namespace Mods

using Mods::fill_with_random;
using Mods::get_random;
using Mods::get_random_uniform;