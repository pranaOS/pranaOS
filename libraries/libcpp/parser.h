/**
 * @file parser.h
 * @author Krisna Pranav
 * @brief parser
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "mods/nonnullrefptr.h"
#include "ast.h"
#include "preprocessor.h"
#include <mods/noncopyable.h>
#include <libcpp/lexer.h>

namespace Cpp
{

    class Parser final 
    {
        MOD_MAKE_NONCOPYABLE(Parser);

    private:
        enum class DeclarationType {
            Function,
            Variable,
            Enum,
            Class,
            Namespace,
            Constructor,
            Destructor
        }; // enum class DeclarationType

        /**
         * @return Optional<DeclarationType> 
         */
        Optional<DeclarationType> match_declaration_in_translation_unit();

        /**
         * @param class_name 
         * @return Optional<Parser::DeclarationType> 
         */
        Optional<Parser::DeclarationType> match_class_member(StringView class_name);

        bool match_function_declaration();
        bool match_comment();
        bool match_preprocessor();
        bool match_whitespace();
        bool match_variable_declaration();
        bool match_expression();
        bool match_secondary_expression();
        bool match_enum_declaration();
        bool match_class_declaration();
        bool match_literal();
        bool match_unary_expression();
        bool match_boolean_literal();
        bool match_keyword(String const&);
        bool match_block_statement();
        bool match_namespace_declaration();
        bool match_template_arguments();
        bool match_name();
        bool match_cpp_cast_expression();
        bool match_c_style_cast_expression();
        bool match_sizeof_expression();
        bool match_braced_init_list();
        bool match_type();
        bool match_named_type();
        bool match_access_specifier();

        /**
         * @param class_name 
         * @return true 
         * @return false 
         */
        bool match_constructor(StringView class_name);

        /**
         * @param class_name 
         * @return true 
         * @return false 
         */
        bool match_destructor(StringView class_name);

        /**
         * @brief Create a root ast node object
         * 
         * @param start 
         * @param end 
         * @return NonnullRefPtr<TranslationUnit> 
         */
        NonnullRefPtr<TranslationUnit> create_root_ast_node(Position const& start, Position end)
        {
            auto node = adopt_ref(*new TranslationUnit(nullptr, start, end, m_filename));
            m_nodes.append(node);
            m_root_node = node;
            return node;
        }

        /**
         * @brief Get the dummy node object
         * 
         * @return DummyAstNode& 
         */
        DummyAstNode& get_dummy_node()
        {
            static NonnullRefPtr<DummyAstNode> dummy = adopt_ref(*new DummyAstNode(nullptr, {}, {}, {}));
            return dummy;
        }

        bool match_attribute_specification();
        void consume_attribute_specification();
        void consume_access_specifier();
        bool match_ellipsis();
        Vector<StringView> parse_type_qualifiers();
        Vector<StringView> parse_function_qualifiers();

        enum class CtorOrDtor {
            Ctor,
            Dtor,
        }; // enum class CtorOrDtor
        
        void parse_constructor_or_destructor_impl(FunctionDeclaration&, CtorOrDtor);

        String m_filename;
        Vector<Token> m_tokens;
        State m_state;
        Vector<State> m_saved_states;
        RefPtr<TranslationUnit> m_root_node;
        Vector<String> m_errors;
        NonnullRefPtrVector<ASTNode> m_nodes;
    }; // class Parser final 
} // namespace Cpp
