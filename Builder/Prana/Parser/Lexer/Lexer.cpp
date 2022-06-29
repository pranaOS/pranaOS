#include "Lexer.h"
#include <iostream>

Lexer::Lexer(const std::string& path) 
    : m_path(path)
    {
        m_stream.open(path, std::ifstream::in);
    }

Lexer::~Lexer()
{
    if (m_stream.is_open()) {
        m_stream.close();
    }
}

Lexer::Lexer(Lexer&& lexer) noexcept 
{
    *this = std::move(lexer);
}

Lexer& Lexer::operator=(Lexer&& lexer) noexcept
{
    m_path = std::move(lexer.m_path);
    m_stream = std::move(lexer.m_stream);
    m_tokens = std::move(lexer.m_tokens);
    return *this;
}