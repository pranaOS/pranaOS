/**
 * @file binary_search.h
 * @author Krisna Pranav
 * @brief Binary_Search
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "span.h"
#include "stdlibextra.h"
#include "types.h"

namespace Mods {

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr int 
     */
    template<typename T>
    constexpr int integral_compare(const typename RemoveConst<T>::Type& a, const typename RemoveConst<T>::Type& b) {
        return a - b;
    }

    /**
     * @tparam T 
     * @tparam Compare 
     * @param haystack 
     * @param needle 
     * @param compare 
     * @param nearby_index 
     * @return constexpr T* 
     */
    template<typename T, typename Compare>
    constexpr T* binary_search(Span<T> haystack, const typename RemoveConst<T>::Type& needle, Compare compare = integral_compare, size_t* nearby_index = nullptr) {
        if (haystack.size() == 0) {
            if (nearby_index)
                *nearby_index = 0;
            return nullptr;
        }

        size_t low = 0;
        size_t high = haystack.size() - 1;
        while (low <= high) {
            size_t middle = low + ((high - low) / 2);
            int comparison = compare(needle, haystack[middle]);
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
    }

}

using Mods::binary_search;