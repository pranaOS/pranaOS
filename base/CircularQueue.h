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

    T dequeue()
    {
        VERIFY(!is_empty());
        auto& slot = elements()[m_head];
        T value = move(slot);
        slot.~T();
        m_head = (m_head + 1) % Capacity;
        --m_size;
        return value;
    }

    const T& at(size_t index) const { return elements()[(m_head + index) % Capacity]; }

    const T& first() const { return at(0); }
    const T& last() const { return at(size() - 1); }

    class ConstIterator {
    public:
        bool operator!=(const ConstIterator& other) { return m_index != other.m_index; }
        ConstIterator& operator++()
        {
            m_index = (m_index + 1) % Capacity;
            if (m_index == m_queue.m_head)
                m_index = m_queue.m_size;
            return *this;
        }

        const T& operator*() const { return m_queue.elements()[m_index]; }

    };
};

}