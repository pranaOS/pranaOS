/**
 * @file parser.cpp
 * @author Krisna Pranav
 * @brief parser
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "parser.h"
#include "ast.h"
#include <mods/debug.h>
#include <mods/scopeguard.h>
#include <mods/scopelogger.h>
#include <libcpp/lexer.h>

#define LOG_SCOPE() ScopeLogger<CPP_DEBUG> logger(String::formatted(""));

namespace Cpp
{

    /**
     * @brief Construct a new Parser::Parser object
     * 
     * @param tokens 
     * @param filename 
     */
    Parser::Parser(Vector<Token> tokens, String const& filename)
        : m_filename(filename)
        , m_tokens(move(tokens))
    {
        if constexpr (CPP_DEBUG) {
            dbgln("Tokens: ");

            for (size_t i = 0; i < m_tokens.size(); ++i) {
                dbgln("{}- {}", i, m_tokens[i].to_string());
            }
        }
    }

    NonnullRefPtr<TranslationUnit> Parser::parse()
    {
        LOG_SCOPE();

        if (m_tokens.is_empty())
            return create_root_ast_node({}, {});
    }

    /**
     * @return true 
     * @return false 
     */
    bool Parser::match_access_specifier()
    {
        if (peek(1).type() != Token::Type::Colon)
            return false;

        return match_keyword("private") || match_keyword("protected") || match_keyword("public");
    }

    void Parser::consume_access_specifier()
    {
        consume(Token::Type::Keyword);
        consume(Token::Type::Colon);
    }

    /**
     * @param class_name 
     * @return true 
     * @return false 
     */
    bool Parser::match_constructor(StringView class_name)
    {
        save_state();
        ScopeGuard state_guard = [this] { load_state(); };

        auto token = consume();
        if (token.text() != class_name)
            return false;

        if (!peek(Token::Type::LeftParen).has_value())
            return false;
        consume();

        while (consume().type() != Token::Type::RightParen && !eof()) { };

        return (peek(Token::Type::Semicolon).has_value() || peek(Token::Type::LeftCurly).has_value());
    }

    /**
     * @param class_name 
     * @return true 
     * @return false 
     */
    bool Parser::match_destructor(StringView class_name)
    {
        save_state();
        ScopeGuard state_guard = [this] { load_state(); };

        if (match_keyword("virtual"))
            consume();

        if (!match(Token::Type::Tilde))
            return false;
        consume();

        auto token = peek();

        if (token.text() != class_name)
            return false;
        consume();

        if (!peek(Token::Type::LeftParen).has_value())
            return false;
        consume();

        while (consume().type() != Token::Type::RightParen && !eof()) { };

        if (match_keyword("override"))
            consume();

        return (peek(Token::Type::Semicolon).has_value() || peek(Token::Type::LeftCurly).has_value());
    }

    /**
     * @param func 
     * @param type 
     */
    void Parser::parse_constructor_or_destructor_impl(FunctionDeclaration& func, CtorOrDtor type)
    {
        if (type == CtorOrDtor::Dtor) {
            if (match_keyword("virtual"))
                func.set_qualifiers({ consume().text() });
            consume(Token::Type::Tilde);
        }

        auto name = parse_name(func);
        func.set_name(name);

        consume(Token::Type::LeftParen);
        auto parameters = parse_parameter_list(func);
        if (parameters.has_value()) {
            if (type == CtorOrDtor::Dtor && !parameters->is_empty())
                error("Destructor declaration that takes parameters");
            else
                func.set_parameters(parameters.value());
        }

        consume(Token::Type::RightParen);

        if (type == CtorOrDtor::Dtor && match_keyword("override"))
            consume();

        RefPtr<FunctionDefinition> body;
        Position ctor_end {};
        if (peek(Token::Type::LeftCurly).has_value()) {
            body = parse_function_definition(func);
            ctor_end = body->end();
        } else {
            ctor_end = position();
            if (match_attribute_specification())
                consume_attribute_specification(); 
            consume(Token::Type::Semicolon);
        }

        func.set_definition(move(body));
        func.set_end(ctor_end);
    }

    /**
     * @param parent 
     * @return NonnullRefPtr<Constructor> 
     */
    NonnullRefPtr<Constructor> Parser::parse_constructor(ASTNode& parent)
    {
        auto ctor = create_ast_node<Constructor>(parent, position(), {});
        parse_constructor_or_destructor_impl(*ctor, CtorOrDtor::Ctor);
        return ctor;
    }

    /**
     * @param parent 
     * @return NonnullRefPtr<Destructor> 
     */
    NonnullRefPtr<Destructor> Parser::parse_destructor(ASTNode& parent)
    {
        auto ctor = create_ast_node<Destructor>(parent, position(), {});
        parse_constructor_or_destructor_impl(*ctor, CtorOrDtor::Dtor);
        return ctor;
    }

} // namespace Cpp
