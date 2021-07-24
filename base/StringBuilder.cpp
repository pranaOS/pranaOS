/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/ByteBuffer.h>
#include <base/Checked.h>
#include <base/Memory.h>
#include <base/PrintfImplementation.h>
#include <base/StdLibExtras.h>
#include <base/String.h>
#include <base/StringBuilder.h>
#include <base/StringView.h>
#include <base/Utf32View.h>

namespace Base {

inline void StringBuilder::will_append(size_t size)
{
    Checked<size_t> needed_capacity = m_buffer.size();
    needed_capacity += size;
    VERIFY(!needed_capacity.has_overflow());
    if (needed_capacity <= m_buffer.capacity())
        return;
    Checked<size_t> expanded_capacity = needed_capacity;
    expanded_capacity *= 2;
    VERIFY(!expanded_capacity.has_overflow());
    m_buffer.ensure_capacity(expanded_capacity.value());
}

StringBuilder::StringBuilder(size_t initial_capacity)
{
    m_buffer.ensure_capacity(initial_capacity);
}

void StringBuilder::append(const StringView& str)
{
    if (str.is_empty())
        return;
    will_append(str.length());
    m_buffer.append(str.characters_without_null_termination(), str.length());
}

void StringBuilder::append(const char* characters, size_t length)
{
    append(StringView { characters, length });
}

void StringBuilder::append(char ch)
{
    will_append(1);
    m_buffer.append(&ch, 1);
}

void StringBuilder::appendvf(const char* fmt, va_list ap)
{
    printf_internal([this](char*&, char ch) {
        append(ch);
    },
        nullptr, fmt, ap);
}

ByteBuffer StringBuilder::to_byte_buffer() const
{
    return ByteBuffer::copy(data(), length());
}

String StringBuilder::to_string() const
{
    if (is_empty())
        return String::empty();
    return String((const char*)data(), length());
}

String StringBuilder::build() const
{
    return to_string();
}

StringView StringBuilder::string_view() const
{
    return StringView { data(), m_buffer.size() };
}

void StringBuilder::clear()
{
    m_buffer.clear();
}


}