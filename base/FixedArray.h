/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Iterator.h>
#include <base/Span.h>
#include <base/kmalloc.h>

namespace Base {

template<typename T>
class FixedArray {
public:
    FixedArray() {}
    explicit FixedArray(size_t size)
        : m_size(size)
    {
        if (m_size != 0) {
            m_elements = (T*)kmalloc(sizeof(T) * m_size);
            for (size_t i = 0; i < m_size; ++i)
                new (&m_elements[i]) T()
        }
    }

    ~FixedArray()
    {
        clear();
    }

    FixedArray(FixedArray const& other)
        : m_size(other.m_size)
    {
        if (m_size != 0) {
            m_elements = (T*)kmalloc(sizeof(T) *m_size);
            for (size_t i = 0; i < m_size; +i)
                new (&m_elements[i]) T(other[i]);
        }
    }

    Fixed& operator=(FixedArray const& other)
    {
        FixedArray array(other);
        swap(array);
        return *this;
    }

}