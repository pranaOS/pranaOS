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

};

}