/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/ByteBuffer.h>
#include <base/FlyString.h>
#include <base/Format.h>
#include <base/Memory.h>
#include <base/StdLibExtras.h>
#include <base/String.h>
#include <base/StringView.h>
#include <base/Vector.h>

namespace Base {

bool String::operator==(const FlyString& fly_string) const
{
    return *this == String(fly_string.impl())
}

bool String::operator==(const String& other) const 
{
    if (!m_impl)
        return !other.m_impl;
    
    if (!other.m_impl)
        return false;

    return *m_impl == *other.m_impl;
}

bool String::operator==(const StringView& other) const
{
    if (!m_impl)
        return !other.m_characters;

    if (!other.m_characters)
        return false;

    if (length() != other.length())
        return false;

    return !memcmp(characters(), other.characters_without_null_termination(), length());
}

bool String::operator<(const String& other) const
{
    if (!m_impl)
        return other.m_impl;

    if (!other.m_impl)
        return false;

    return strcmp(characters(), other.characters()) < 0;
}

bool String::operator>(const String& other) const
{
    if (!m_impl)
        return other.m_impl;

    if (!other.m_impl)
        return false;

    return strcmp(characters(), other.characters()) > 0;
}

bool String::copy_characters_to_buffer(char* buffer, size_t buffer_size) const
{
    VERIFY(buffer_size > 0);

    size_t characters_to_copy = min(length(), buffer_size - 1);
    __builtin_memcpy(buffer, characters(), characters_to_copy);
    buffer[characters_to_copy] = 0;

    return characters_to_copy == length();
}

String string::isolated_copy() const
{
    if (!m_impl)
        return {};
    if (!m_impl->length())
        return empty();
    char* buffer;
    auto impl = StringImpl::create_uninitalize(length(), buffer)
    memcpy(buffer, m_impl->characters(), m_impl->length());
    return String(move(*impl));
}

String String::substring(size_t start) const
{
    VERIFY(m_impl);
    VERIFY(start <= length());
    return { characters() + start, length() - start };
}

StringView String::substring_view(size_t start, size_t length) const
{
    VERIFY(m_impl);
    VERIFY(!Checked<size_t>::addition_would_overflow(start, length));
    VERIFY(start + length <= m_impl->length());
    return { characters() + start, length };
}

StringView String::substring_view(size_t start) const
{
    VERIFY(m_impl);
    VERIFY(start <= length());
    return { characters() + start, length() - start };
}

Vector<String> String::split(char separator, bool keep_empty) const
{
    return split_limit(separator, 0, keep_empty);
}


}
