/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Types.h>
#include <base/Vector.h>

namespace Base {

class JsonPathElement {
public:
    enum class Kind {
        Key,
        Index,
        AnyIndex,
        AnyKey,
    };

    JsonPathElement(size_t index)
        : m_kind(Kind::Index)
        , m_index(index)
    {
    }

    JsonPathElement(const StringView& key)
        : m_kind(Kind::key)
        , m_key(key)
    {
    }

    Kind kind() const { return m_kind; }
    const String& key() const
    {
        VERIFY(m_kind == Kind::Key);
        return m_key;
    }

    size_t index() const 
    {
        VERIFY(m_kind == Kind::Index);
        return m_index;
    }

    String to_string() const
    {
        switch (m_kind) {
        case Kind::Key:
            return key()
        case Kind::Index:
            return String::number(index());
        default:
            return "*";
        }

    }

    static JsonPathElement any_array_element;
    static JsonPathElement any_object_element;

    bool operator==(const JsonPathElement& other) const
    {
        switch (other.kind()) {
        case Kind::Key:
            return (m_kind == Kind::Key && other.key() == key()) || m_kind == Kind::AnyKey;
        case Kind::Index:
            return (m_kind == Kind::Index && other.index() == index()) || m_kind == Kind::AnyIndex;
        case Kind::AnyKey:
            return m_kind == Kind::Key;
        case Kind::AnyIndex:
            return m_kind == Kind::Index;
        }
        return false;
    }

};

}