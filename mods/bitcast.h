/**
 * @file bitcast.h
 * @author Krisna Pranav
 * @brief bit cast
 * @version 6.0
 * @date 2024-08-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

namespace Mods 
{

    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @return constexpr T 
     */
    template<typename T, typename U>
    [[nodiscard]] constexpr inline T bit_cast(U const& a)
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

}

#if USING_MODS_GLOBALLY
using Mods::bit_cast;
#endif
