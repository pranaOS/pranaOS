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


};