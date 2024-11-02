/**
 * @file regexlexer.h
 * @author Krisna Pranav
 * @brief Regex Lexer
 * @version 6.0
 * @date 2024-11-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/genericlexer.h>
#include <mods/string_view.h>

namespace regex 
{
    #define ENUMERATE_REGEX_TOKENS              \
        __ENUMERATE_REGEX_TOKEN(Eof)            \
        __ENUMERATE_REGEX_TOKEN(Char)           \
        __ENUMERATE_REGEX_TOKEN(Circumflex)     \
        __ENUMERATE_REGEX_TOKEN(Period)         \
        __ENUMERATE_REGEX_TOKEN(LeftParen)      \
        __ENUMERATE_REGEX_TOKEN(RightParen)     \
        __ENUMERATE_REGEX_TOKEN(LeftCurly)      \
        __ENUMERATE_REGEX_TOKEN(RightCurly)     \
        __ENUMERATE_REGEX_TOKEN(LeftBracket)    \
        __ENUMERATE_REGEX_TOKEN(RightBracket)   \
        __ENUMERATE_REGEX_TOKEN(Asterisk)       \
        __ENUMERATE_REGEX_TOKEN(EscapeSequence) \
        __ENUMERATE_REGEX_TOKEN(Dollar)         \
        __ENUMERATE_REGEX_TOKEN(Pipe)           \
        __ENUMERATE_REGEX_TOKEN(Plus)           \
        __ENUMERATE_REGEX_TOKEN(Comma)          \
        __ENUMERATE_REGEX_TOKEN(Slash)          \
        __ENUMERATE_REGEX_TOKEN(EqualSign)      \
        __ENUMERATE_REGEX_TOKEN(HyphenMinus)    \
        __ENUMERATE_REGEX_TOKEN(Colon)          \
        __ENUMERATE_REGEX_TOKEN(Questionmark)

    enum class TokenType 
    {
    #define __ENUMERATE_REGEX_TOKEN(x) x,
        ENUMERATE_REGEX_TOKENS
    #undef __ENUMERATE_REGEX_TOKEN
    }; // enum class TokenType

    class Token 
    {
    public:
        /**
         * @brief Construct a new Token object
         * 
         */
        Token() = default;

        /**
         * @brief Construct a new Token object
         * 
         * @param type 
         * @param start_position 
         * @param value 
         */
        Token(TokenType const type, size_t const start_position, StringView const value)
            : m_type(type)
            , m_position(start_position)
            , m_value(value)
        {
        }

        /**
         * @return TokenType 
         */
        TokenType type() const 
        { 
            return m_type; 
        }

        /**
         * @return StringView 
         */
        StringView value() const 
        { 
            return m_value; 
        }

        /**
         * @return size_t 
         */
        size_t position() const 
        { 
            return m_position; 
        }

        char const* name() const;
        static char const* name(TokenType);

    private:
        TokenType m_type { TokenType::Eof };
        size_t m_position { 0 };
        StringView m_value { nullptr };
    }; // class Token

    class Lexer : public GenericLexer 
    {
    public:
        /**
         * @brief Construct a new Lexer object
         * 
         */
        Lexer();

        /**
         * @brief Construct a new Lexer object
         * 
         * @param source 
         */
        explicit Lexer(StringView source);

        /**
         * @return Token 
         */
        Token next();

        void reset();

        /**
         * @param offset 
         */
        void back(size_t offset);

        char consume();

        /**
         * @brief Set the source object
         * 
         * @param source 
         */
        void set_source(StringView const source) 
        { 
            m_input = source; 
        }

        auto const& source() const 
        { 
            return m_input; 
        }

    private:
        size_t m_previous_position { 0 };
        Token m_current_token { TokenType::Eof, 0, StringView(nullptr) };
    }; // class Lexer : public GenericLexer
} // namespace regex

using regex::Lexer;
