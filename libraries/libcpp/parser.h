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
    }; // class Parser final 
} // namespace Cpp
