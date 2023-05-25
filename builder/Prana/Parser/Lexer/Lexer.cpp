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
    m_line_idx = lexer.m_line_idx;
    m_cur_line = std::move(lexer.m_cur_line);
    return *this;
}

void Lexer::run()
{
    while (next_string()) {
        int nesting = calc_nesting_for_cur_line();
        std::string cur_word {};

        while (char c = eat()) {
            if (c == '\\') {
                cur_word.push_back(eat());
                continue;
            }

            auto token = Token::TokenFromChar(c, nesting, m_token_line);

            if (token.type() == Token::Type::Default) {
                cur_word.push_back(c);
                continue;
            }

            if (token.type() == Token::Type::VariableBegin) {
                if (!cur_word.empty()) {
                    m_tokens.emplace_back(cur_word, Token::Type::Default, nesting, m_token_line);
                    cur_word.clear();
                }
                while (char var_c = eat()) {
                    if (Token::token_type(var_c) == Token::Type::VariableEnd) {
                        break;
                    }
                    cur_word.push_back(var_c);
                }
                token.m_content = std::make_shared<std::string>(cur_word);
                token.m_type = Token::Type::Variable;
                m_tokens.push_back(token);
                m_variables.push_back(m_tokens.size() - 1);
                eat_spaces();
                cur_word.clear();
                continue;
            }

            if (!cur_word.empty()) {
                m_tokens.emplace_back(cur_word, Token::Type::Default, nesting, m_token_line);
                cur_word.clear();
            }
            m_tokens.push_back(token);
            eat_spaces();
        }

        if (!cur_word.empty()) {
            m_tokens.emplace_back(cur_word, Token::Type::Default, nesting, m_token_line);
        }
    }
}
int Lexer::calc_nesting_for_cur_line()
{
    int nesting = 0;
    while (char cur = lookup()) {
        if (cur == ' ') {
            eat();
            if (chars_remain() < 3) {
                return nesting;
            }
            for (size_t i = 0; i < 3; i++) {
                if (lookup() != ' ') {
                    return nesting;
                }
                eat();
            }
            nesting++;
            continue;
        }

        if (cur == '\n') {
            nesting++;
            eat();
            continue;
        }

        return nesting;
    }

    return nesting;
}

bool Lexer::next_string()
{
    m_line_idx = 0;
    m_token_line++;
    return bool(getline(m_stream, m_cur_line));
}

void Lexer::eat_spaces()
{
    while (char c = lookup()) {
        if (c != ' ') {
            return;
        }
        eat();
    }
}