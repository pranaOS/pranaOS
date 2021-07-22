/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

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
    Optional<JsonValue> parser_helper();

};

}