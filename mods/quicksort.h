/**
 * @file quicksort.h
 * @author Krisna Pranav
 * @brief QUICKSORT!
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "stdlibextra.h"

namespace Mods
{

    /**
     * @tparam Collection 
     * @tparam LessThan 
     * @param col 
     * @param start 
     * @param end 
     * @param less_than 
     */
    template<typename Collection, typename LessThan>
    void dual_pivot_quick_sort(Collection& col, int start, int end, LessThan less_than)
    {
        if (start >= end) {
            return;
        }

        int left_pointer, right_pointer;

        if (!less_than(col[start], col[end])) {
            swap(col[start], col[end]);
        }

        int j = start + 1;
        int k = start + 1;
        int g = end - 1;

        auto&& left_pivot = col[start];
        auto&& right_pivot = col[end];
    }

    /**
     * @tparam Iterator 
     * @param start 
     * @param end 
     */
    template<typename Iterator>
    void quick_sort(Iterator start, Iterator end)
    {
        quick_sort(start, end, [](auto& a, auto& b) { return a < b });
    }

    /**
     * @tparam Collection 
     * @tparam LessThan 
     * @param collection 
     * @param less_than 
     */
    template<typename Collection, typename LessThan>
    void quick_sort(Collection& collection, LessThan less_than)
    {
        dual_pivot_quick_sort(collection, 0, collection.size() - 1, move(less_than));
    }

    /**
     * @tparam Collection 
     * @param collection 
     */
    template<typename Collection>
    void quick_sort(Collection& collection)
    {
        dual_pivot_quick_sort(collection, 0, collection.size() - 1, [](auto& a, auto& b) { return a < b});
    }

} // namespace Mods