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
    

};

}