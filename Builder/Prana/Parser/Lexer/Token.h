#pragma once

#include <memory>
#include <string>
#include <utility>

class Parser;
class Lexer;

class Token {
    friend class Parser;
    friend class Lexer;

public:
    enum class Type {
        Default,
        SubRule,
        Comma,
        Equal,
        VariableBegin,
        VariableEnd,
        Variable,
    };  

public:
    Token(std::string content, Type type, int nesting, size_t line)
        : m_content(new std::string(std::move(content)))
        , m_type(type)
        , m_nesting(nesting)
        , m_line(line)
    {
    }

    Token(Type type, int nesting, size_t line)
        : m_type(type)
        , m_nesting(nesting)
        , m_line(line)
    {
    }

    static Token TokenFromChar(char c, int nesting, size_t line)
    {
        return Token(token_type(c), nesting, line);
    }

    static Type token_type(char c)
    {
        switch (c) {
        case ',':
            return Type::Comma;
        case ':':
            return Type::SubRule;
        case '~':
            return Type::Equal;
        case '{':
            return Type::VariableBegin;
        case '}':
            return Type::VariableEnd;
        }
        return Type::Default;
    }

    auto& content() {
        return m_content;
    }

    Type type() const { return m_type; }
    int nesting() const { return m_nesting; }
    size_t line() const { return m_line; }


private:
    std::shared_ptr<std::string> m_content {};
    Type m_type;
    int m_nesting;

    size_t m_line {};

};