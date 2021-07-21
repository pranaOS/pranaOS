/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/JsonArraySerializer.h>
#include <base/JsonValue.h>
#include <base/Vector.h>

namespace Base {

class JsonArray {
public:
    JsonArray() = default;
    ~JsonArray() = default;

    JsonArray(JsonArray const& other)
        : m_values(other.m_values)
    {
    }

    JsonArray(JsonArray&& other)
        : m_values(move(other.m_values))
    {
    }

    template<typename T>
    JsonArray(Vector<T> const& vector)
    {
        for (auto& value : vector)
            m_values.append(move(value));
    }

    JsonArray& operator=(JsonArray const& other)
    {
        if (this != &other)
            m_values = other.m_values;
        return *this;
    }

};

}
