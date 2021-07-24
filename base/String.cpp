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


}