/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Forward.h>
#include <base/StdLibExtras.h>

namespace Base {

template<typename T, size_t Capacity>
class CircularQueue {
    friend CircularDuplexStream<Capacity>;

public:
    CircularQueue()
    {
    }

    ~CircularQueue()
    {
        clear();
    }

    void clear()
    {
        for (size_t i = 0; i < m_size; ++i)
            elements()[(m_head + i) % Capacity].~T();

        m_head = 0;
        m_size = 0;
    }

    bool is_empty() const { return !m_size; }
    size_t size() const { return m_size; }

    size_t capacity() const { return Capacity; }

    template<typename U = T>
    void enqueue(U&& value)
    {
        auto& slot = elements()[(m_head + m_size) % Capacity];
        if (m_size == Capacity)
            slot.~T();

        new (&slot) T(forward<U>(value));
        if (m_size == Capacity)
            m_head = (m_head + 1) % Capacity;
        else
            ++m_size;
    }

};

}