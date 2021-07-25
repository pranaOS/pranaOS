/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Forward.h>
#include <base/Optional.h>
#include <base/Span.h>
#include <base/String.h>
#include <base/Types.h>
#include <base/Vector.h>

namespace Base {

Vector<u16> utf8_to_utf16(StringView const&);
Vector<u16> utf8_to_utf16(Utf8View const&);
Vector<u16> utf32_to_utf16(Utf32View const&);

class Utf16View;

class Utf16CodePointIterator {
    friend class Utf16View;

public:
    Utf16CodePointIterator() = default;
    ~Utf16CodePointIterator() = default;

    bool operator==(Utf16CodePointIterator const& other) const
    {
        return (m_ptr == other.m_ptr) && (m_remaining_code_units == other.m_remaining_code_units);
    }

    bool operator!=(Utf16CodePointIterator const& other) const
    {
        return !(*this == other);
    }

    Utf16CodePointIterator& operator++();
    u32 operator*() const;

    size_t length_in_code_units() const;

private:
    Utf16CodePointIterator(u16 const* ptr, size_t length)
        : m_ptr(ptr)
        , m_remaining_code_units(length)
    {
    }

    u16 const* m_ptr { nullptr };
    size_t m_remaining_code_units { 0 };
};

}