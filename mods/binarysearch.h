/**
 * @file binarysearch.h
 * @author Krisna Pranav
 * @brief Binary Search
 * @version 6.0
 * @date 2025-02-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextra.h>
#include <mods/types.h>

namespace Mods
{
    struct DefaultComparator
    {
        /**
         * @tparam T 
         * @tparam S 
         * @param lhs 
         * @param rhs 
         * @return constexpr int 
         */
        template<typename T, typename S>
        constexpr int operator()(T& lhs, S& rhs)
        {
            if (lhs > rhs) 
                return 1;
            
            if (lhs < rhs)
                return -1;
            
            return 0;
        }
    }; // struct DefaultComparator
} // namespace Mods