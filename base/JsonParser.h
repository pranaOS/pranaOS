/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/GenericLexer.h>
#include <base/JsonValue.h>

namespace Base {

class JsonParser : private GenericLexer {
public:
    explicit JsonParser(const StringView& input)
        : GenericLexer(input)
    {
    }

    Optional<JsonValue> parse();

private:
    Optional<JsonValue> parse_helper();

    String consume_and_unescape_string();
    Optional<JsonValue> parse_array();
    Optional<JsonValue> parse_object();
    Optional<JsonValue> parse_number();
    Optional<JsonValue> parse_string();
    Optional<JsonValue> parse_false();
    Optional<JsonValue> parse_true();
    Optional<JsonValue> parse_null();

    String m_last_string_starting_with_character[256];
};

}

using Base::JsonParser;