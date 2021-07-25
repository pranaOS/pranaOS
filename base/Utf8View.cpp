/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <base/Format.h>
#include <base/Utf8View.h>

namespace Base {

Utf8View::Utf8View(const String& string)
    : m_string(string)
{
}

Utf8View::Utf8View(const StringView& string)
    : m_string(string)
{
}

Utf8View::Utf8View(const char* string)
    : m_string(string)
{
}

const unsigned char* Utf8View::begin_ptr() const
{
    return (const unsigned char*)m_string.characters_without_null_termination();
}

const unsigned char* Utf8View::end_ptr() const
{
    return begin_ptr() + m_string.length();
}

Utf8CodePointIterator Utf8View::begin() const
{
    return { begin_ptr(), m_string.length() };
}

Utf8CodePointIterator Utf8View::end() const
{
    return { end_ptr(), 0 };
}

Utf8CodePointIterator Utf8View::iterator_at_byte_offset(size_t byte_offset) const
{
    size_t current_offset = 0;
    for (auto iterator = begin(); !iterator.done(); ++iterator) {
        if (current_offset >= byte_offset)
            return iterator;
        current_offset += iterator.underlying_code_point_length_in_bytes();
    }
    return end();
}

size_t Utf8View::byte_offset_of(const Utf8CodePointIterator& it) const
{
    VERIFY(it.m_ptr >= begin_ptr());
    VERIFY(it.m_ptr <= end_ptr());

    return it.m_ptr - begin_ptr();
}

Utf8View Utf8View::substring_view(size_t byte_offset, size_t byte_length) const
{
    StringView string = m_string.substring_view(byte_offset, byte_length);
    return Utf8View { string };
}

Utf8View Utf8View::unicode_substring_view(size_t code_point_offset, size_t code_point_length) const
{
    if (code_point_length == 0)
        return {};

    size_t code_point_index = 0, offset_in_bytes = 0;
    for (auto iterator = begin(); !iterator.done(); ++iterator) {
        if (code_point_index == code_point_offset)
            offset_in_bytes = byte_offset_of(iterator);
        if (code_point_index == code_point_offset + code_point_length - 1) {
            size_t length_in_bytes = byte_offset_of(++iterator) - offset_in_bytes;
            return substring_view(offset_in_bytes, length_in_bytes);
        }
        ++code_point_index;
    }

    VERIFY_NOT_REACHED();
}

static inline bool decode_first_byte(
    unsigned char byte,
    size_t& out_code_point_length_in_bytes,
    u32& out_value)
{
    if ((byte & 128) == 0) {
        out_value = byte;
        out_code_point_length_in_bytes = 1;
        return true;
    }
    if ((byte & 64) == 0) {
        return false;
    }
    if ((byte & 32) == 0) {
        out_value = byte & 31;
        out_code_point_length_in_bytes = 2;
        return true;
    }
    if ((byte & 16) == 0) {
        out_value = byte & 15;
        out_code_point_length_in_bytes = 3;
        return true;
    }
    if ((byte & 8) == 0) {
        out_value = byte & 7;
        out_code_point_length_in_bytes = 4;
        return true;
    }

    return false;
}

bool Utf8View::validate(size_t& valid_bytes) const
{
    valid_bytes = 0;
    for (auto ptr = begin_ptr(); ptr < end_ptr(); ptr++) {
        size_t code_point_length_in_bytes;
        u32 value;
        bool first_byte_makes_sense = decode_first_byte(*ptr, code_point_length_in_bytes, value);
        if (!first_byte_makes_sense)
            return false;

        for (size_t i = 1; i < code_point_length_in_bytes; i++) {
            ptr++;
            if (ptr >= end_ptr())
                return false;
            if (*ptr >> 6 != 2)
                return false;
        }

        valid_bytes += code_point_length_in_bytes;
    }

    return true;
}

size_t Utf8View::calculate_length() const
{
    size_t length = 0;
    for ([[maybe_unused]] auto code_point : *this) {
        ++length;
    }
    return length;
}

bool Utf8View::starts_with(const Utf8View& start) const
{
    if (start.is_empty())
        return true;
    if (is_empty())
        return false;
    if (start.length() > length())
        return false;
    if (begin_ptr() == start.begin_ptr())
        return true;

    for (auto k = begin(), l = start.begin(); l != start.end(); ++k, ++l) {
        if (*k != *l)
            return false;
    }
    return true;
}


}