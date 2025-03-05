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

    public:
        /**
         * @brief Construct a new Parser object
         * 
         * @param tokens 
         * @param filename 
         */
        explicit Parser(Vector<Token> tokens, String const& filename);

        /**
         * @brief Destroy the Parser object
         * 
         */
        ~Parser() = default;

        /**
         * @return NonnullRefPtr<TranslationUnit> 
         */
        NonnullRefPtr<TranslationUnit> parse();

        /**
         * @return true 
         * @return false 
         */
        bool eof() const;

        /**
         * @return RefPtr<ASTNode> 
         */
        RefPtr<ASTNode> node_at(Position) const;

        /**
         * @return Optional<size_t> 
         */
        Optional<size_t> index_of_node_at(Position) const;
        Optional<Token> token_at(Position) const;
        Optional<size_t> index_of_token_at(Position) const;

        /**
         * @return RefPtr<const TranslationUnit> 
         */
        RefPtr<const TranslationUnit> root_node() const 
        { 
            return m_root_node; 
        }
        
        String text_of_node(ASTNode const&) const;

        /**
         * @param token 
         * @return StringView 
         */
        StringView text_of_token(Cpp::Token const& token) const;

        void print_tokens() const;

        /**
         * @return Vector<Token> const& 
         */
        Vector<Token> const& tokens() const 
        { 
            return m_tokens; 
        }

        /**
         * @return Vector<String> const& 
         */
        Vector<String> const& errors() const 
        { 
            return m_errors; 
        }

        struct TodoEntry {
            String content;
            String filename;
            size_t line { 0 };
            size_t column { 0 };
        }; // struct TodoEntry

        /**
         * @brief Get the todo entries object
         * 
         * @return Vector<TodoEntry> 
         */
        Vector<TodoEntry> get_todo_entries() const;

        /**
         * @param start 
         * @param end 
         * @return Vector<Token> 
         */
        Vector<Token> tokens_in_range(Position start, Position end) const;

    private:
        enum class DeclarationType {
            Function,
            Variable,
            Enum,
            Class,
            Namespace,
            Constructor,
            Destructor,
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

        /**
         * @return true 
         * @return false 
         */
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
        bool match_destructor(StringView class_name);

        /**
         * @param parent 
         * @return Optional<NonnullRefPtrVector<Parameter>> 
         */
        Optional<NonnullRefPtrVector<Parameter>> parse_parameter_list(ASTNode& parent);

        /**
         * @return Optional<Token> 
         */
        Optional<Token> consume_whitespace();

        void consume_preprocessor();

        /**
         * @param parent 
         * @return NonnullRefPtr<Declaration> 
         */
        NonnullRefPtr<Declaration> parse_declaration(ASTNode& parent, DeclarationType);

        /**
         * @param parent 
         * @return NonnullRefPtr<FunctionDeclaration> 
         */
        NonnullRefPtr<FunctionDeclaration> parse_function_declaration(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<FunctionDefinition> 
         */
        NonnullRefPtr<FunctionDefinition> parse_function_definition(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Statement> 
         */
        NonnullRefPtr<Statement> parse_statement(ASTNode& parent);

        /**
         * @param parent 
         * @param expect_semicolon 
         * @return NonnullRefPtr<VariableDeclaration> 
         */
        NonnullRefPtr<VariableDeclaration> parse_variable_declaration(ASTNode& parent, bool expect_semicolon = true);

        /**
         * @param parent 
         * @return NonnullRefPtr<Expression> 
         */
        NonnullRefPtr<Expression> parse_expression(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Expression> 
         */
        NonnullRefPtr<Expression> parse_primary_expression(ASTNode& parent);

        /**
         * @param parent 
         * @param lhs 
         * @return NonnullRefPtr<Expression> 
         */
        NonnullRefPtr<Expression> parse_secondary_expression(ASTNode& parent, NonnullRefPtr<Expression> lhs);

        /**
         * @param parent 
         * @return NonnullRefPtr<FunctionCall> 
         */
        NonnullRefPtr<FunctionCall> parse_function_call(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<StringLiteral> 
         */
        NonnullRefPtr<StringLiteral> parse_string_literal(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<ReturnStatement> 
         */
        NonnullRefPtr<ReturnStatement> parse_return_statement(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<EnumDeclaration> 
         */
        NonnullRefPtr<EnumDeclaration> parse_enum_declaration(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<StructOrClassDeclaration> 
         */
        NonnullRefPtr<StructOrClassDeclaration> parse_class_declaration(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Expression> 
         */
        NonnullRefPtr<Expression> parse_literal(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<UnaryExpression> 
         */
        NonnullRefPtr<UnaryExpression> parse_unary_expression(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<BooleanLiteral> 
         */
        NonnullRefPtr<BooleanLiteral> parse_boolean_literal(ASTNode& parent);

        /**
         * @brief 
         * 
         * @param parent 
         * @return NonnullRefPtr<Type> 
         */
        NonnullRefPtr<Type> parse_type(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<NamedType> 
         */
        NonnullRefPtr<NamedType> parse_named_type(ASTNode& parent);

        /**
         * @param parent 
         * @param lhs 
         * @return NonnullRefPtr<BinaryExpression> 
         */
        NonnullRefPtr<BinaryExpression> parse_binary_expression(ASTNode& parent, NonnullRefPtr<Expression> lhs, BinaryOp);

        /**
         * @param parent 
         * @param lhs 
         * @return NonnullRefPtr<AssignmentExpression> 
         */
        NonnullRefPtr<AssignmentExpression> parse_assignment_expression(ASTNode& parent, NonnullRefPtr<Expression> lhs, AssignmentOp);

        /**
         * @param parent 
         * @return NonnullRefPtr<ForStatement> 
         */
        NonnullRefPtr<ForStatement> parse_for_statement(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<BlockStatement> 
         */
        NonnullRefPtr<BlockStatement> parse_block_statement(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Comment> 
         */
        NonnullRefPtr<Comment> parse_comment(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<IfStatement> 
         */
        NonnullRefPtr<IfStatement> parse_if_statement(ASTNode& parent);

        /**
         * @param parent 
         * @param is_nested_namespace 
         * @return NonnullRefPtr<NamespaceDeclaration> 
         */
        NonnullRefPtr<NamespaceDeclaration> parse_namespace_declaration(ASTNode& parent, bool is_nested_namespace = false);

        /**
         * @param parent 
         * @return NonnullRefPtrVector<Declaration> 
         */
        NonnullRefPtrVector<Declaration> parse_declarations_in_translation_unit(ASTNode& parent);

        /**
         * @param parent 
         * @return RefPtr<Declaration> 
         */
        RefPtr<Declaration> parse_single_declaration_in_translation_unit(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtrVector<Type> 
         */
        NonnullRefPtrVector<Type> parse_template_arguments(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Name> 
         */
        NonnullRefPtr<Name> parse_name(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<CppCastExpression> 
         */
        NonnullRefPtr<CppCastExpression> parse_cpp_cast_expression(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<SizeofExpression> 
         */
        NonnullRefPtr<SizeofExpression> parse_sizeof_expression(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<BracedInitList> 
         */
        NonnullRefPtr<BracedInitList> parse_braced_init_list(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<CStyleCastExpression> 
         */
        NonnullRefPtr<CStyleCastExpression> parse_c_style_cast_expression(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtrVector<Declaration> 
         */
        NonnullRefPtrVector<Declaration> parse_class_members(StructOrClassDeclaration& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Constructor> 
         */
        NonnullRefPtr<Constructor> parse_constructor(ASTNode& parent);

        /**
         * @param parent 
         * @return NonnullRefPtr<Destructor> 
         */
        NonnullRefPtr<Destructor> parse_destructor(ASTNode& parent);

        bool match(Token::Type);
        Token consume(Token::Type);
        Token consume();

        Token consume_keyword(String const&);

        /**
         * @param offset 
         * @return Token 
         */
        Token peek(size_t offset = 0) const;

        Optional<Token> peek(Token::Type) const;

        /**
         * @return Position 
         */
        Position position() const;
        Position previous_token_end() const;

        /**
         * @param start 
         * @param end 
         * @return String 
         */
        String text_in_range(Position start, Position end) const;

        void save_state();
        void load_state();

        struct State {
            size_t token_index { 0 };
            NonnullRefPtrVector<ASTNode> state_nodes;
        }; // struct State 

        /**
         * @param message 
         */
        void error(StringView message = {});

        /**
         * @brief Create a ast node object
         * 
         * @tparam T 
         * @tparam Args 
         * @param parent 
         * @param start 
         * @param end 
         * @param args 
         * @return NonnullRefPtr<T> 
         */
        template<class T, class... Args>
        NonnullRefPtr<T>
        create_ast_node(ASTNode& parent, Position const& start, Optional<Position> end, Args&&... args)
        {
            auto node = adopt_ref(*new T(&parent, start, end, m_filename, forward<Args>(args)...));

            if (m_saved_states.is_empty()) {
                m_nodes.append(node);
            } else {
                m_state.state_nodes.append(node);
            }

            return node;
        }

        /**
         * @brief Create a root ast node object
         * 
         * @param start 
         * @param end 
         * @return NonnullRefPtr<TranslationUnit> 
         */
        NonnullRefPtr<TranslationUnit>
        create_root_ast_node(Position const& start, Position end)
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

        /**
         * @return Vector<StringView> 
         */
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
