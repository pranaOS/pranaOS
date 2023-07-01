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

        bool is_eof() const;

        /**
         * @param offset 
         * @return char 
         */
        char peek(size_t offset = 0) const;

        /**
         * @return true 
         * @return false 
         */
        bool next_is(char) const;

        /**
         * @return true 
         * @return false 
         */
        bool next_is(StringView) const;

        /**
         * @return StringView 
         */
        StringView consume_all();
        StringView consume(size_t count);
        StringView consume_until(char);
        StringView consume_until(const char*);

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
         * @brief consume_while
         * 
         * @tparam C 
         * @param condition 
         * @return StringView 
         */
        template<typename C>
        StringView consume_while(C condition) {
            size_t start = m_index;

            while(!is_eof() && condition(peek()))
                m_index++;
            
            size_t length = m_index - start;

            if (length == 0)
                return {};
            
            return m_input.substring_view(start, length);
        }

    protected:
        StringView m_input;
        size_t m_index { 0 };
    };
}