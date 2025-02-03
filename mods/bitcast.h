/**
 * @file bitcast.h
 * @author Krisna Pranav
 * @brief bit cast
 * @version 6.0
 * @date 2024-08-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods 
{
    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @return T 
     */
    template<typename T, typename U>
    inline T bit_cast(const U& a)
    {
    #if (__has_builtin(__builtin_bit_cast))
        return __builtin_bit_cast(T, a);
    #else
        static_assert(sizeof(T) == sizeof(U));

        T result;
        __builtin_memcpy(&result, &a, sizeof(T));
        return result;
    #endif
    }

} // namspace Mods

using Mods::bit_cast;
