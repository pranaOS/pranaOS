/**
 * @file random.h
 * @author Krisna Pranav
 * @brief Random
 * @version 6.0
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/platform.h>

#if defined(__prana__)
#    include <stdlib.h>
#endif

#if defined(__unix__)
#    include <unistd.h>
#endif

#if defined(__APPLE__)
#    include <sys/random.h>
#endif

namespace Mods
{   
    /**
     * @param buffer 
     * @param length 
     */
    inline void fill_with_random(void* buffer, size_t length)
    {
        #if defined(__prana__)
            arc4random_buf(buffer, length);
        #elif defined(OSS_FUZZ)
            (void)buffer;
            (void)length;
        #elif defined(__unix__) or defined(__APPLE__)
            int rc = getentropy(buffer, length);
            (void)rc;
        #endif
    }

    /**
     * @tparam T
    */
    template<typename T>
    inline T get_random()
    {
        T t;
        fill_with_random(&t, sizeof(T));
        return t;
    }

} // namespace Mods

using Mods::fill_with_random;
using Mods::get_random;