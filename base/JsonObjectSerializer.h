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
    void add(const StringView& key, const char* value)
    {
        begin_item(key);
        m_builder.append('"');
        m_builder.append_escaped_for_json(value);
        m_builder.append('"');
    }

    void add(const StringView& key, bool value)
    {
        begin_item(key);
        m_builder.append(value ? "true" : "false");
    }

    void add(const StringView& key, int value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }

    void add(const StringView& key, unsigned value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }

    void add(const StringView& key, long value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }

    void add(const StringView& key, long unsigned value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }

    void add(const StringView& key, long long value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }

    void add(const StringView& key, long long unsigned value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }

#ifndef KERNEL
    void add(const StringView& key, double value)
    {
        begin_item(key);
        m_builder.appendff("{}", value);
    }
#endif

}

}