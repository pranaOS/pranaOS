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

#include <mods/stdlibextra.h>

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
    template <typename Collection, typename LessThan>
    void dual_pivot_quick_sort(Collection& col, int start, int end, LessThan less_than)
    {
        while(start < end)
        {
            int size = end - start + 1;
            if(size > 3)
            {
                int third = size / 3;
                if(less_than(col[start + third], col[end - third]))
                {
                    swap(col[start + third], col[start]);
                    swap(col[end - third], col[end]);
                }
                else
                {
                    swap(col[start + third], col[end]);
                    swap(col[end - third], col[start]);
                }
            }
            else
            {
                if(!less_than(col[start], col[end]))
                {
                    swap(col[start], col[end]);
                }
            }

            int j = start + 1;
            int k = start + 1;
            int g = end - 1;

            auto&& left_pivot = col[start];
            auto&& right_pivot = col[end];

            while(k <= g)
            {
                if(less_than(col[k], left_pivot))
                {
                    swap(col[k], col[j]);
                    j++;
                }
                else if(!less_than(col[k], right_pivot))
                {
                    while(!less_than(col[g], right_pivot) && k < g)
                    {
                        g--;
                    }
                    swap(col[k], col[g]);
                    g--;
                    if(less_than(col[k], left_pivot))
                    {
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

            int left_pointer = j;
            int right_pointer = g;

            int left_size = left_pointer - start;
            int middle_size = right_pointer - (left_pointer + 1);
            int right_size = (end + 1) - (right_pointer + 1);

            if(left_size >= middle_size && left_size >= right_size)
            {
                dual_pivot_quick_sort(col, left_pointer + 1, right_pointer - 1, less_than);
                dual_pivot_quick_sort(col, right_pointer + 1, end, less_than);
                end = left_pointer - 1;
            }
            else if(middle_size >= right_size)
            {
                dual_pivot_quick_sort(col, start, left_pointer - 1, less_than);
                dual_pivot_quick_sort(col, right_pointer + 1, end, less_than);
                start = left_pointer + 1;
                end = right_pointer - 1;
            }
            else
            {
                dual_pivot_quick_sort(col, start, left_pointer - 1, less_than);
                dual_pivot_quick_sort(col, left_pointer + 1, right_pointer - 1, less_than);
                start = right_pointer + 1;
            }
        }
    }

    /**
     * @tparam Iterator 
     * @tparam LessThan 
     * @param start 
     * @param end 
     * @param less_than 
     */
    template <typename Iterator, typename LessThan>
    void single_pivot_quick_sort(Iterator start, Iterator end, LessThan less_than)
    {
        for(;;)
        {
            int size = end - start;
            if(size <= 1)
                return;

            int pivot_point = size / 2;
            if(pivot_point)
                swap(*(start + pivot_point), *start);

            auto&& pivot = *start;

            int i = 1;
            for(int j = 1; j < size; ++j)
            {
                if(less_than(*(start + j), pivot))
                {
                    swap(*(start + j), *(start + i));
                    ++i;
                }
            }

            swap(*start, *(start + i - 1));
            
            if(i > size / 2)
            {
                single_pivot_quick_sort(start + i, end, less_than);
                end = start + i - 1;
            }
            else
            {
                single_pivot_quick_sort(start, start + i - 1, less_than);
                start = start + i;
            }
        }
    }

    /**
     * @tparam Iterator 
     * @param start 
     * @param end 
     */
    template <typename Iterator>
    void quick_sort(Iterator start, Iterator end)
    {
        single_pivot_quick_sort(start, end, [](auto& a, auto& b)
                                { return a < b; });
    }

    /**
     * @tparam Iterator 
     * @tparam LessThan 
     * @param start 
     * @param end 
     * @param less_than 
     */
    template <typename Iterator, typename LessThan>
    void quick_sort(Iterator start, Iterator end, LessThan less_than)
    {
        single_pivot_quick_sort(start, end, move(less_than));
    }

    /**
     * @tparam Collection 
     * @tparam LessThan 
     * @param collection 
     * @param less_than 
     */
    template <typename Collection, typename LessThan>
    void quick_sort(Collection& collection, LessThan less_than)
    {
        dual_pivot_quick_sort(collection, 0, collection.size() - 1, move(less_than));
    }

    /**
     * @tparam Collection 
     * @param collection 
     */
    template <typename Collection>
    void quick_sort(Collection& collection)
    {
        dual_pivot_quick_sort(collection, 0, collection.size() - 1, [](auto& a, auto& b)
                            { return a < b; });
    }

} // namespace Mods

using Mods::quick_sort;