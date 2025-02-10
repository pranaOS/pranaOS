/**
 * @file jsonparser.h
 * @author Krisna Pranav
 * @brief Json Parser
 * @version 6.0
 * @date 2025-02-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/genericlexer.h>
#include <mods/jsonvalue.h>

namespace Mods
{
    class JsonParser : private GenericLexer 
    {
    public:
        /**
         * @brief Construct a new Json Parser object
         * 
         * @param input 
         */
        explicit JsonParser(StringView input)
            : GenericLexer(input)
        {}
    private:
        ErrorOr<JsonValue> parse_helper();
        ErrorOr<String> consume_and_unescape_string();
        ErrorOr<JsonValue> parse_array();
        ErrorOr<JsonValue> parse_number();
        ErrorOr<JsonValue> parse_string();
        ErrorOr<JsonValue> parse_false();
        ErrorOr<JsonValue> parse_true();
    }; // class JsonParser : private GenericLexer 
} // namespace Mods