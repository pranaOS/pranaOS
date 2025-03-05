/**
 * @file preprocessor.h
 * @author Krisna Pranav
 * @brief pre processor
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/flystring.h>
#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/optional.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/vector.h>
#include <libcpp/token.h>

namespace Cpp
{

    class Preprocessor 
    {
    private:
        void handle_preprocessor_statement(StringView);
        void handle_include_statement(StringView);

        /**
         * @param keyword 
         */
        void handle_preprocessor_keyword(StringView keyword);
        
        /**
         * @param value 
         * @return String 
         */
        String remove_escaped_newlines(StringView value);

        struct MacroCall {
            Token name;

            struct Argument {
                Vector<Token> tokens;
            }; // struct Argument

            Vector<Argument> arguments;
            size_t end_token_index { 0 };
        }; // struct MacroCall

        enum class State
        {
            Normal,
            SkipIfBranch,
            SkipElseBranch
        }; // enum class State

        State m_state { State::Normal };

        Vector<StringView> m_included_paths;

        struct Options {
            bool ignore_unsupported_keywords { false };
            bool ignore_invalid_statements { false };
            bool keep_invalid_statements { false };
        }; // struct Options 
        
    }; // class Preprocessor 

} // namespace Cpp
