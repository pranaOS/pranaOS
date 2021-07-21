/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/JsonArraySerializer.h>

#ifndef KERNEL
#    include <base/JsonValue.h>
#endif

namespace Base {

template<typename Builder>
class JsonObjectSerializer {
public:
    explicit JsonObjectSerializer(Builder& builder)
        : m_builder(builder)
    {
        m_builder.append('{');
    }

    JsonObjectSerializer(const JsonObjectSerializer&) = delete;
    JsonObjectSerializer(JsonObjectSerializer&&) = delete;

    ~JsonObjectSerializer()
    {
        if (!m_finished)
            finish();
    }

#ifndef KERNEL
    void add(const StringView& key, const JsonValue& value)
    {
        begin_item(key);
        value.serialize(m_builder);
    }
#endif

}

}