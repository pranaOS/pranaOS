/**
 * @file preprocessor.cpp
 * @author Krisna Pranav
 * @brief pre processor
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "preprocessor.h"
#include <mods/assertions.h>
#include <mods/genericlexer.h>
#include <mods/stringbuilder.h>
#include <libcpp/lexer.h>
#include <ctype.h>

namespace Cpp
{
    
    /**
     * @brief Construct a new Preprocessor::Preprocessor object
     * 
     * @param filename 
     * @param program 
     */
    Preprocessor::Preprocessor(String const& filename, StringView program)
        : m_filename(filename)
        , m_program(program)
    {
    }

    /**
     * @return Vector<Token> 
     */
    Vector<Token> Preprocessor::process_and_lex()
    {
        Lexer lexer { m_program };
        lexer.set_ignore_whitespace(true);
        auto tokens = lexer.lex();

        m_unprocessed_tokens = tokens;

        for (size_t token_index = 0; token_index < tokens.size(); ++token_index) {
            auto& token = tokens[token_index];
            m_current_line = token.start().line;
            if (token.type() == Token::Type::PreprocessorStatement) {
                handle_preprocessor_statement(token.text());
                m_processed_tokens.append(tokens[token_index]);
                continue;
            }

            if (m_state != State::Normal)
                continue;

            if (token.type() == Token::Type::IncludeStatement) {
                if (token_index >= tokens.size() - 1 || tokens[token_index + 1].type() != Token::Type::IncludePath)
                    continue;
                handle_include_statement(tokens[token_index + 1].text());
                if (m_options.keep_include_statements) {
                    m_processed_tokens.append(tokens[token_index]);
                    m_processed_tokens.append(tokens[token_index + 1]);
                }
                ++token_index; 
                continue;
            }

            if (token.type() == Token::Type::Identifier) {
                if (auto defined_value = m_definitions.find(token.text()); defined_value != m_definitions.end()) {
                    auto last_substituted_token_index = do_substitution(tokens, token_index, defined_value->value);
                    token_index = last_substituted_token_index;
                    continue;
                }
            }

            m_processed_tokens.append(token);
        }

        return m_processed_tokens;
    }

    /**
     * @param lexer 
     */
    static void consume_whitespace(GenericLexer& lexer)
    {
        auto ignore_line = [&] {
            for (;;) {
                if (lexer.consume_specific("\\\n"sv)) {
                    lexer.ignore(2);
                } else {
                    lexer.ignore_until('\n');
                    break;
                }
            }
        };
        for (;;) {
            if (lexer.consume_specific("//"sv))
                ignore_line();
            else if (lexer.consume_specific("/*"sv))
                lexer.ignore_until("*/");
            else if (lexer.next_is("\\\n"sv))
                lexer.ignore(2);
            else if (lexer.is_eof() || !lexer.next_is(isspace))
                break;
            else
                lexer.ignore();
        }
    }

    /**
     * @param line 
     */
    void Preprocessor::handle_preprocessor_statement(StringView line)
    {
        GenericLexer lexer(line);

        consume_whitespace(lexer);
        lexer.consume_specific('#');
        consume_whitespace(lexer);
        auto keyword = lexer.consume_until(' ');
        lexer.ignore();
        if (keyword.is_empty() || keyword.is_null() || keyword.is_whitespace())
            return;

        handle_preprocessor_keyword(keyword, lexer);
    }

} // namespace Cpp
