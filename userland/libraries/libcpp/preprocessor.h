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
    public:
        /**
         * @brief Construct a new Preprocessor object
         * 
         * @param filename 
         * @param program 
         */
        explicit Preprocessor(String const& filename, StringView program);

        /**
         * @return Vector<Token> 
         */
        Vector<Token> process_and_lex();

        /**
         * @return Vector<StringView> 
         */
        Vector<StringView> included_paths() const 
        { 
            return m_included_paths; 
        }

        struct Definition {
            String key;
            Vector<String> parameters;
            String value;
            FlyString filename;
            size_t line { 0 };
            size_t column { 0 };
        }; // struct Definition

        using Definitions = HashMap<String, Definition>;

        struct Substitution {
            Vector<Token> original_tokens;
            Definition defined_value;
            String processed_value;
        }; // struct Substitution

        /**
         * @return Definitions const& 
         */
        Definitions const& definitions() const 
        { 
            return m_definitions; 
        }

        /**
         * @return Vector<Substitution> const& 
         */
        Vector<Substitution> const& substitutions() const 
        { 
            return m_substitutions; 
        }
        
        /**
         * @brief Set the ignore unsupported keywords object
         * 
         * @param ignore 
         */
        void set_ignore_unsupported_keywords(bool ignore) 
        { 
            m_options.ignore_unsupported_keywords = ignore; 
        }

        /**
         * @brief Set the ignore invalid statements object
         * 
         * @param ignore 
         */
        void set_ignore_invalid_statements(bool ignore) 
        { 
            m_options.ignore_invalid_statements = ignore; 
        }

        /**
         * @brief Set the keep include statements object
         * 
         * @param keep 
         */
        void set_keep_include_statements(bool keep) 
        { 
            m_options.keep_include_statements = keep; 
        }

        Function<Definitions(StringView)> definitions_in_header_callback { nullptr };

        /**
         * @return Vector<Token> const& 
         */
        Vector<Token> const& unprocessed_tokens() const 
        { 
            return m_unprocessed_tokens; 
        }

    private:
        void handle_preprocessor_statement(StringView);
        void handle_include_statement(StringView);

        /**
         * @param keyword 
         * @param line_lexer 
         */
        void handle_preprocessor_keyword(StringView keyword, GenericLexer& line_lexer);
        
        /**
         * @param value 
         * @return String 
         */
        String remove_escaped_newlines(StringView value);

        /**
         * @param tokens 
         * @param token_index 
         * @return size_t 
         */
        size_t do_substitution(Vector<Token> const& tokens, size_t token_index, Definition const&);

        /**
         * @brief Create a definition object
         * 
         * @param line 
         * @return Optional<Definition> 
         */
        Optional<Definition> create_definition(StringView line);

        struct MacroCall {
            Token name;
            
            struct Argument {
                Vector<Token> tokens;
            }; // struct Argument

            Vector<Argument> arguments;
            size_t end_token_index { 0 };
        }; // struct MacroCall

        /**
         * @param tokens 
         * @param token_index 
         * @return Optional<MacroCall> 
         */
        Optional<MacroCall> parse_macro_call(Vector<Token> const& tokens, size_t token_index);

        /**
         * @return String 
         */
        String evaluate_macro_call(MacroCall const&, Definition const&);

        String m_filename;
        String m_program;

        Vector<Token> m_unprocessed_tokens;
        Vector<Token> m_processed_tokens;
        Definitions m_definitions;
        Vector<Substitution> m_substitutions;

        size_t m_current_line { 0 };
        size_t m_current_depth { 0 };
        Vector<size_t> m_depths_of_taken_branches;
        Vector<size_t> m_depths_of_not_taken_branches;

        enum class State {
            Normal,
            SkipIfBranch,
            SkipElseBranch
        }; // enum class State

        State m_state { State::Normal };

        Vector<StringView> m_included_paths;

        struct Options {
            bool ignore_unsupported_keywords { false };
            bool ignore_invalid_statements { false };
            bool keep_include_statements { false };
        } m_options;
    }; // class Preprocessor 

} // namespace Cpp
