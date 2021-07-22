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

};

}