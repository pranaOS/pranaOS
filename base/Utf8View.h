/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StringView.h>
#include <base/Types.h>

namespace Base {

class Utf8View;

class Utf8CodePointIterator {
    friend class Utf8View;

public:
    Utf8CodePointIterator() = default;
    ~Utf8CodePointIterator() = default;

    bool operator==(const Utf8CodePointIterator&) const;
    bool operator!=(const Utf8CodePointIterator&) const;
    Utf8CodePointIterator& operator++();
    u32 operator*() const;

    Optional<u32> peek(size_t offset = 0) const;

    ssize_t operator-(const Utf8CodePointIterator& other) const
    {
        return m_ptr - other.m_ptr;
    }

    size_t underlying_code_point_length_in_bytes() const;
    ReadonlyBytes underlying_code_point_bytes() const;
    bool done() const { return m_length == 0; }

private:
    Utf8CodePointIterator(const unsigned char*, size_t);
    const unsigned char* m_ptr { nullptr };
    size_t m_length;
};

}