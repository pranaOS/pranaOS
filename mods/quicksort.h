/**
 * @file quicksort.h
 * @author Krisna Pranav
 * @brief QUICKSORT!
 * @version 6.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

        while (k <= g) {
            if (less_than(col[k], left_pivot)) {
                swap(col[k], col[j]);
                j++;
            } else if (!less_than(col[k], right_pivot)) {
                while (!less_than(col[g], right_pivot) && k < g) {
                    g--;
                }
                swap(col[k], col[g]);
                g--;
                if (less_than(col[k], left_pivot)) {
                    swap(col[k], col[j]);
                    j++;
                }
            }
            k++;
        }
        j--;
        g++;

        swap(col[start], col[j]);
        swap(col[end], col[g]);

        left_pointer = j;
        right_pointer = g;

        dual_pivot_quick_sort(col, start, left_pointer - 1, less_than);
        dual_pivot_quick_sort(col, left_pointer + 1, right_pointer - 1, less_than);
        dual_pivot_quick_sort(col, right_pointer + 1, end, less_than);
    }

    /**
     * @tparam Iterator 
     * @tparam LessThan 
     * @param start 
     * @param end 
     * @param less_than 
     */
    template<typename Iterator, typename LessThan>
    void quick_sort(Iterator start, Iterator end, LessThan less_than)
    {
        int size = end - start;
        if (size <= 1)
            return;

        int pivot_point = size / 2;
        auto pivot = *(start + pivot_point);

        if (pivot_point)
            swap(*(start + pivot_point), *start);

        int i = 1;
        for (int j = 1; j < size; ++j) {
            if (less_than(*(start + j), pivot)) {
                swap(*(start + j), *(start + i));
                ++i;
            }
        }

        swap(*start, *(start + i - 1));
        quick_sort(start, start + i - 1, less_than);
        quick_sort(start + i, end, less_than);
    }

    /**
     * @tparam Iterator 
     * @param start 
     * @param end 
     */
    template<typename Iterator>
    void quick_sort(Iterator start, Iterator end)
    {
        quick_sort(start, end, [](auto& a, auto& b) { return a < b; });
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
        dual_pivot_quick_sort(collection, 0, collection.size() - 1,
            [](auto& a, auto& b) { return a < b; });
    }

}

using Mods::quick_sort;