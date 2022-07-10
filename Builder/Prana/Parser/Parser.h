#pragma once

#include "../Utils/Logger.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

#pragma once
using TokenProcessor = const std::function<void(const Token&)>;

class Context;

class Parser {
public:
    Parser() = default;
    Parser(const Parser&) = default;
}