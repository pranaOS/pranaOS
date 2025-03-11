/**
 * @file token.h
 * @author Krisna Pranav
 * @brief token
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stringview.h>
#include <mods/types.h>

namespace Cpp 
{

#define FOR_EACH_TOKEN_TYPE        \
    __TOKEN(Unknown)               \
    __TOKEN(Whitespace)            \
    __TOKEN(PreprocessorStatement) \
    __TOKEN(IncludeStatement)      \
    __TOKEN(IncludePath)           \
    __TOKEN(LeftParen)             \
    __TOKEN(RightParen)            \
    __TOKEN(LeftCurly)             \
    __TOKEN(RightCurly)            \
    __TOKEN(LeftBracket)           \
    __TOKEN(RightBracket)          \
    __TOKEN(Less)                  \
    __TOKEN(Greater)               \
    __TOKEN(LessEquals)            \
    __TOKEN(GreaterEquals)         \
    __TOKEN(LessLess)              \
    __TOKEN(GreaterGreater)        \
    __TOKEN(LessLessEquals)        \
    __TOKEN(GreaterGreaterEquals)  \
    __TOKEN(LessGreater)           \
    __TOKEN(Comma)                 \
    __TOKEN(Plus)                  \
    __TOKEN(PlusPlus)              \
    __TOKEN(PlusEquals)            \
    __TOKEN(Minus)                 \
    __TOKEN(MinusMinus)            \
    __TOKEN(MinusEquals)           \
    __TOKEN(Asterisk)              \
    __TOKEN(AsteriskEquals)        \
    __TOKEN(Slash)                 \
    __TOKEN(SlashEquals)           \
    __TOKEN(Percent)               \
    __TOKEN(PercentEquals)         \
    __TOKEN(Caret)                 \
    __TOKEN(CaretEquals)           \
    __TOKEN(ExclamationMark)       \
    __TOKEN(ExclamationMarkEquals) \
    __TOKEN(Equals)                \
    __TOKEN(EqualsEquals)          \
    __TOKEN(And)                   \
    __TOKEN(AndAnd)                \
    __TOKEN(AndEquals)             \
    __TOKEN(Pipe)                  \
    __TOKEN(PipePipe)              \
    __TOKEN(PipeEquals)            \
    __TOKEN(Tilde)                 \
    __TOKEN(QuestionMark)          \
    __TOKEN(Colon)                 \
    __TOKEN(ColonColon)            \
    __TOKEN(ColonColonAsterisk)    \
    __TOKEN(Semicolon)             \
    __TOKEN(Dot)                   \
    __TOKEN(DotAsterisk)           \
    __TOKEN(Arrow)                 \
    __TOKEN(ArrowAsterisk)         \
    __TOKEN(DoubleQuotedString)    \
    __TOKEN(SingleQuotedString)    \
    __TOKEN(RawString)             \
    __TOKEN(EscapeSequence)        \
    __TOKEN(Comment)               \
    __TOKEN(Integer)               \
    __TOKEN(Float)                 \
    __TOKEN(Keyword)               \
    __TOKEN(KnownType)             \
    __TOKEN(Identifier)            \
    __TOKEN(EOF_TOKEN)

struct Position {
    size_t line { 0 };
    size_t column { 0 };

    /**
     * @return true 
     * @return false 
     */
    bool operator<(Position const&) const;
    bool operator<=(Position const&) const;
    bool operator>(Position const&) const;
    bool operator==(Position const&) const;
}; // struct Position

struct Token {
    enum class Type {
#define __TOKEN(x) x,
        FOR_EACH_TOKEN_TYPE
#undef __TOKEN
    }; // enum class  Type

    /**
     * @brief Construct a new Token object
     * 
     * @param type 
     * @param start 
     * @param end 
     * @param text 
     */
    Token(Type type, Position const& start, Position const& end, StringView text)
        : m_type(type)
        , m_start(start)
        , m_end(end)
        , m_text(text)
    {
    }

    /**
     * @param t 
     * @return char const* 
     */
    static char const* type_to_string(Type t)
    {
        switch (t) {
#define __TOKEN(x) \
    case Type::x:  \
        return #x;
            FOR_EACH_TOKEN_TYPE
#undef __TOKEN
        }
        VERIFY_NOT_REACHED();
    }

    /**
     * @return String 
     */
    String to_string() const;
    String type_as_string() const;

    /**
     * @return Position const& 
     */
    Position const& start() const 
    { 
        return m_start; 
    }

    /**
     * @return Position const& 
     */
    Position const& end() const 
    { 
        return m_end; 
    }

    /**
     * @brief Set the start object
     * 
     * @param other 
     */
    void set_start(Position const& other) 
    { 
        m_start = other; 
    }

    /**
     * @brief Set the end object
     * 
     * @param other 
     */
    void set_end(Position const& other) 
    { 
        m_end = other; 
    }

    /**
     * @return Type 
     */
    Type type() const 
    { 
        return m_type; 
    }

    /**
     * @return StringView 
     */
    StringView text() const 
    { 
        return m_text; 
    }

private:
    Type m_type { Type::Unknown };
    Position m_start;
    Position m_end;
    StringView m_text;
}; // struct Token
 
} // namespace Cpp

#undef FOR_EACH_TOKEN_TYPE
