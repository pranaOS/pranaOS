#pragma once

#include "../Utils/Logger.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include <functional>

#pragma once
using TokenProcessor = const std::function<void(const Token&)>;
using TokenContentProcessor = const std::function<void(const std::shared_ptr<std::string>& content)>&;

class Context;

class Parser {
public:
    Parser() = default;
    Parser(const Parser&) = default;
    
    void run();
    
private:
    void parse_include();
    void parse_defines();
    void parse_commands();
    void parse_build();
    void parse_deault();
}
