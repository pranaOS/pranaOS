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

    /**
     * @tparam Container 
     * @tparam Needle 
     * @tparam Comparator 
     * @param haystack 
     * @param needle 
     * @param nearby_index 
     * @param comparator 
     * @return decltype(&haystack[0]) 
     */
    template<typename Container, typename Needle, typename Comparator = DefaultComparator>
    constexpr auto binary_search(Container&& haystack, Needle&& needle, size_t* nearby_index = nullptr, Comparator comparator = Comparator {}) -> decltype(&haystack[0])
    {
        if (haystack.size() == 0) {
            if (nearby_index)
                *nearby_index = 0;

            return nullptr;
        }

        size_t low = 0;
        size_t high = haystack.size() - 1;
        
        while (low <= high) {
            size_t middle = low + (high - low) / 2;

            int comparison = comparator(needle, haystack[middle]);

            if (comparison < 0)
                if (middle != 0)
                    high = middle - 1;
                else
                    break;
            else if (comparison > 0)
                low = middle + 1;
            else {
                if (nearby_index)
                    *nearby_index = middle;
                return &haystack[middle];
            }
        }

        if (nearby_index)
            *nearby_index = min(low, high);

        return nullptr;
    } // constexpr auto binary_search(Container&& haystack, Needle&& needle, size_t* nearby_index = nullptr, Comparator comparator = Comparator {}) -> decltype(&haystack[0])
}

using Mods::binary_search;
