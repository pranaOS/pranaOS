/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Base 
{

template <typename K, typename V, size_t Capacity> 
class BinaryHeap
{
public:
    BinaryHeap() = default;
    ~BinaryHeap() = default;

    BinaryHeap(K keys[], V values[], size_t size)
    {
        VERIFY(size <= Capacity);
        m_size = size;
        for (size_t i = 0; i < size; i++) {
            m_elements[i].key = keys[i];
            m_elements[i].value = values[i];
        }

        for (ssize_t i = size / 2; i >= 0; i--) {
            heapify_down(i);
        }
    }
    
};

}