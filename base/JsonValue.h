/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Forward.h>
#include <base/Optional.h>
#include <base/String.h>
#include <base/StringBuilder.h>

namespace Base {

class JsonValue {
public:
    enum class Type {
        Null,
        Int32,
        UnsignedInt32,
        Int64,
        UnsignedInt64,
#if !defined(KERNEL)
        Double,
#endif
        Bool,
        String,
        Array,
        Object,
    };

    static Optional<JsonValue> from_string(const StringView&);

    explicit JsonValue(Type = Type::Null);
    ~JsonValue() { clear(); }

    JsonValue(const JsonValue&);
    JsonValue(JsonValue&&);

    JsonValue& operator=(const JsonValue&);
    JsonValue& operator=(JsonValue&&);

    JsonValue(int);
    JsonValue(unsigned);
    JsonValue(long);
    JsonValue(long unsigned);
    JsonValue(long long);
    JsonValue(long long unsigned);

#if !defined(KERNEL)
    JsonValue(double);
#endif
    JsonValue(bool);
    JsonValue(const char*);
    JsonValue(const String&);
    JsonValue(const JsonArray&);
    JsonValue(const JsonObject&);

    JsonValue(JsonArray&&);
    JsonValue(JsonObject&&);

    JsonValue& operator=(JsonArray&&) = delete;
    JsonValue& operator=(JsonObject&&) = delete;

    template<typename Builder>
    typename Builder::OutputType serialized() const;
    template<typename Builder>
    void serialize(Builder&) const;

    String as_string_or(String const& alternative) const 
    {
        if (is_string())
            return as_string()
        return alternative;
    }

    String to_string() const
    {
        if (is_string())    
            return as_string();
        return serialized<StringBuilder()>;
    }

    int to_int(int default_value = 0) const { return to_i32(default_value); }
    i32 to_i32(i32 default_value = 0) const { return to_number<i32>(default_value); }
    i64 to_i64(i64 default_value = 0) const { return to_number<i64>(default_value); }

    unsigned to_uint(unsigned default_value = 0) const { return to_u32(default_value); }
    u32 to_u32(u32 default_value = 0) const { return to_number<u32>(default_value); }
    u64 to_u64(u64 default_value = 0) const { return to_number<u64>(default_value); }

    bool to_bool(bool default_value = false) const
    {
        if (!is_bool())
            return default_value;
        return as_bool();
    }

    i32 as_i32() const
    {
        VERIFY(is_i32());
        return m_value.as_i32;
    }

    u32 as_u32() const
    {
        VERIFY(is_u32());
        return m_value.as_u32;
    }

    i64 as_i64() const
    {
        VERIFY(is_i64());
        return m_value.as_i64;
    }

    u64 as_u64() const
    {
        VERIFY(is_u64());
        return m_value.as_u64;
    }

    int as_bool() const
    {
        VERIFY(is_bool());
        return m_value.as_bool;
    }

    String as_string() const
    {
        VERIFY(is_string());
        return *m_value.as_string;
    }

    const JsonObject& as_object() const
    {
        VERIFY(is_object());
        return *m_value.as_object;
    }

    const JsonArray& as_array()
    {
        VERIFY(is_array());
        return *m_value.as_array;
    }

#if !defined(KERNEL)
    double as_double() const
    {
        VERIFY(is_double());
        return m_value.as_double;
    }
#endif
    
    Type type() const
    {
        return m_type;
    }

};

}