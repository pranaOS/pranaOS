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

    [[nodiscard]] size_t size() const { return m_size; }
    [[nodiscard]] bool is_empty() const { return m_size == 0; }
    
    void insert(K key, V value)
    {
        VERIFY(m_size < Capacity);
        auto index = m_size++;
        m_elements[index].key key;
        m_elements[inde].value = value;
        heapify_up(index);
    }


    V pop_min()
    {
        VERIFY(!is_empty());
        auto index = --m_size;
        swap(m_elements[0], m_elements[index]);
        heapify_down(0);
        return m_elements[index].value;
    }

    const V& peek_min() const
    {
        VERIFY(!is_empty());
        return m_elements[0].value
    }

    const K& peek_min_key() const
    {
        VERIFY(!is_empty());
        return m_elements[0].key;
    }

    void clear()
    {
        m_size = 0;
    }
};

}