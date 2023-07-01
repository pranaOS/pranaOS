/**
 * @file genericlexer.h
 * @author Krisna Pranav
 * @brief genericlexer
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "string_view.h"

namespace Mods {

    class GenericLexer {
    public:
        explicit GenericLexer(const StringView& input);
        virtual ~GenericLexer();

        /**
         * @return size_t 
         */
        size_t tell() const { 
            return m_index; 
        }

        /**
         * @return size_t 
         */
        size_t tell_remaining() const { 
            return m_input.length() - m_index; 
        }

        /**
         * @return StringView 
         */
        StringView remaining() const { 
            return m_input.substring_view(m_index); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_eof() const;

        /**
         * @param offset 
         * @return char 
         */
        char peek(size_t offset = 0) const;

        /**
         * @brief next_is
         * 
         * @return true 
         * @return false 
         */
        bool next_is(char) const;
        bool next_is(StringView) const;
        bool next_is(const char*) const;

        void retreat();

        /**
         * @return char 
         */
        char consume();
        bool consume_specific(char);

        /**
         * @return true 
         * @return false 
         */
        bool consume_specific(StringView);
        bool consume_specific(const char*);
        char consume_escaped_character(char escape_char = '\\', const StringView& escape_map = "n\nr\rt\tb\bf\f");

        /**
         * @param count 
         * @return StringView 
         */
        StringView consume(size_t count);

        /**
         * @return StringView 
         */
        StringView consume_all();

        /**
         * @return StringView 
         */
        StringView consume_line();

        /**
         * @return StringView 
         */
        StringView consume_until(char);
        StringView consume_until(const char*);

        /**
         * @param escape_char 
         * @return StringView 
         */
        StringView consume_quoted_string(char escape_char = 0);
        String consume_and_unescape_string(char escape_char = '\\');

        /**
         * @param count 
         */
        void ignore(size_t count = 1);
        void ignore_until(char);
        void ignore_until(const char*);

        /**
         * @tparam C 
         * @param condition 
         * @return true 
         * @return false 
         */
        template<typename C>
        bool next_is(C condition) const {
            return condition(peek());
        }

        /**
         * @tparam C 
         * @param condition 
         * @return StringView 
         */
        template<typename C>
        StringView consume_while(C condition) {
            size_t start = m_index;
            while (!is_eof() && condition(peek()))
                m_index++;
            size_t length = m_index - start;

            if (length == 0)
                return {};
            return m_input.substring_view(start, length);
        }

        /**
         * @tparam C 
         * @param condition 
         * @return StringView 
         */
        template<typename C>
        StringView consume_until(C condition) {
            size_t start = m_index;
            while (!is_eof() && !condition(peek()))
                m_index++;
            size_t length = m_index - start;

            if (length == 0)
                return {};
            return m_input.substring_view(start, length);
        }
        
        /**
         * @tparam C 
         * @param condition 
         */
        template<typename C>
        void ignore_while(C condition) {
            while (!is_eof() && condition(peek()))
                m_index++;
        }

        /**
         * @tparam C 
         * @param condition 
         */
        template<typename C>
        void ignore_until(C condition) {
            while (!is_eof() && !condition(peek()))
                m_index++;
        }

    protected:
        StringView m_input;
        size_t m_index { 0 };
    };

    /**
     * @brief is_any_of
     * 
     * @param values 
     * @return constexpr auto 
     */
    constexpr auto is_any_of(const StringView& values) {
        return [values](auto c) { return values.contains(c); };
    }

    constexpr auto is_path_separator = is_any_of("/\\");
    constexpr auto is_quote = is_any_of("'\"");

}

using Mods::GenericLexer;
using Mods::is_any_of;
using Mods::is_path_separator;
using Mods::is_quote;
