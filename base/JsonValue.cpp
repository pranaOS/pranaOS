/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/JsonArray.h>
#include <base/JsonObject.h>
#include <base/JsonValue.h>

#ifndef KERNEL
#    include <base/JsonParser.h>
#endif


namespace Base {


JsonValue::JsonValue(Type type)
    : m_type(type)
{
}

JsonValue::JsonValue(const JsonValue& other)
{
    copy_from(other);
}

JsonValue& JsonValue::operator=(const JsonValue& other)
{
    if (this != &other) {
        clear();
        copy_from(other);
    }

    return *this;
}

void JsonValue::copy_from(const JsonValue& other)
{
    m_type = other.m_type;
    switch (m_type) {
    case Type::String:
        VERIFY(!m_value.as_string);
        m_value.as_string = other.m_value.as_string;
        m_value.as_string->ref();
        break;
    case Type::Object:
        m_value.as_object = new JsonObject(*other.m_value.as_object);
        break;
    case Type::Array:
        m_value.as_array = new JsonArray(*other.m_value.as_array);
        break;
    default:
        m_value.as_u64 = other.m_value.as_u64;
        break;
    }
}

}