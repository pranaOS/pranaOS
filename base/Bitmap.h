/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/BitmapView.h>
#include <base/Noncopyable.h>
#include <base/Optional.h>
#include <base/Platform.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>
#include <base/kmalloc.h>

namespace Base {

class Bitmap {
    Base_MAKE_NONCOPYABLE(Bitmap);

public:
    Bitmap() = default;

    Bitmap(size_t size, bool default_value)
        : m_size(size)
    {
        VERIFY(m_size != 0);
        m_data = static_cast<u8*>(kmalloc(size_in_bytes()));
        fill(default_value);
    }

    Bitmap(u8* data, size_t size, bool is_owning = false)
        : m_data(data)
        , m_size(size)
        , m_is_owning(is_owning)
    {
    }
    
    BitmapView view() { return { m_data, m_size }; }
    const BitmapView view() const { return { m_data, m_size }; }

    Bitmap(Bitmap&& other)
        : m_data(exchange(other.m_data, nullptr))
        , m_size(exchange(other.m_size, 0))
    {
    }

    Bitmap& operator=(Bitmap&& other)
    {
        if (this != &other) {
            kfree_size(m_data, size_in_bytes());
            m_data = exchange(other.m_data, nullptr);
            m_size = exchange(other.m_size, 0);
        }
        return *this;
    }

    ~Bitmap()
    {
        if (m_is_owning) {
            kfree_sized(m_data, size_in_bytes());
        }
        m_data = nullptr;
    }


    size_t size() const { return m_size; }
    size_t size_in_bytes() const { return ceil_div(m_size, static_cast<size_t>(8)); }

    bool get(size_t index) const
    {
        VERIFY(index < m_size);
        return 0 != (m_data[index / 8] & (1u << (index % 8)));
    }

    void set(size_t index, bool value)
    {
        VERIFY(index < m_size);
        if (value)
            m_data[index / 8] |= static_cast<u8>((1u << (index % 8)));
        else
            m_data[index / 8] &= static_cast<u8>(~(1u << (index % 8)));
    }

    size_t count_slow(bool value) const { return count_in_range(0, m_size, value); }
    size_t count_in_range(size_t start, size_t len, bool value) const { return view().count_in_range(start, len, value); }

    bool is_null() const { return !m_data; }

    u8* data() { return m_data; }
    const u8* data() const { return m_data; }

};

}