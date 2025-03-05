/**
 * @file lexer.h
 * @author Krisna Pranav
 * @brief lexer
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "libcpp/token.h"
#include <mods/stringview.h>
#include <mods/vector.h>

namespace Cpp 
{

    class Lexer 
    {
    public:
        /**
         * @brief Construct a new Lexer object
         * 
         * @param start_line 
         */
        explicit Lexer(StringView, size_t start_line = 0);

        /**
         * @return Vector<Token> 
         */
        Vector<Token> lex();
        template<typename Callback>
        void lex_iterable(Callback);

        /**
         * @brief Set the ignore whitespace object
         * 
         * @param value 
         */
        void set_ignore_whitespace(bool value) 
        { 
            m_options.ignore_whitespace = value; 
        }

    private:
        /**
         * @param offset 
         * @return char 
         */
        char peek(size_t offset = 0) const;

        char consume();
        
        void lex_impl(Function<void(Token)>);

        StringView m_input;
        size_t m_index { 0 };
        Position m_previous_position { 0, 0 };
        Position m_position { 0, 0 };

        struct Options {
            bool ignore_whitespace { false };
        } m_options;
    }; // class Lexer 

    /**
     * @tparam Callback 
     * @param callback 
     */
    template<typename Callback>
    void Lexer::lex_iterable(Callback callback)
    {
        return lex_impl(move(callback));
    }

} // namespace Cpp
