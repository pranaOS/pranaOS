/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Checked.h>
#include <base/Types.h>

namespace Base {
class Utf32View;

class Utf32CodePointIterator {
    friend class Utf32View;

public:
    Utf32CodePointIterator() = default;
    ~Utf32CodePointIterator() = default;

    bool operator==(const Utf32CodePointIterator& other) const
    {
        return m_ptr == other.m_ptr && m_length == other.m_length;
    }
    bool operator!=(const Utf32CodePointIterator& other) const
    {
        return !(*this == other);
    }
    Utf32CodePointIterator& operator++()
    {
        VERIFY(m_length > 0);
        m_ptr++;
        m_length--;
        return *this;
    }
    ssize_t operator-(const Utf32CodePointIterator& other) const
    {
        return m_ptr - other.m_ptr;
    }
    u32 operator*() const
    {
        VERIFY(m_length > 0);
        return *m_ptr;
    }

    constexpr int code_point_length_in_bytes() const { return sizeof(u32); }
    bool done() const { return !m_length; }

private:
    Utf32CodePointIterator(const u32* ptr, size_t length)
        : m_ptr(ptr)
        , m_length((ssize_t)length)
    {
    }
    const u32* m_ptr { nullptr };
    ssize_t m_length { -1 };
};
}