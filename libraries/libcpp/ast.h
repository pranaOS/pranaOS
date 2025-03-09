/**
 * @file ast.h
 * @author Krisna Pranav
 * @brief ast
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/flystring.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/optional.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/typecasts.h>
#include <mods/vector.h>
#include <libcpp/lexer.h>

namespace Cpp 
{

    class ASTNode;
    class TranslationUnit;
    class Declaration;
    class FunctionDefinition;
    class Type;
    class Parameter;
    class Statement;
    class Name;

    class ASTNode : public RefCounted<ASTNode> 
    {
    public:
        /**
         * @brief Destroy the ASTNode object
         * 
         */
        virtual ~ASTNode() = default;

        virtual StringView class_name() const = 0;
        
        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const;

        /**
         * @tparam T 
         * @return true 
         * @return false 
         */
        template<typename T>
        bool fast_is() const = delete;

        /**
         * @return ASTNode* 
         */
        ASTNode* parent() const 
        { 
            return m_parent; 
        }

        /**
         * @return Position 
         */
        Position start() const
        {
            VERIFY(m_start.has_value());
            return m_start.value();
        }
        Position end() const
        {
            VERIFY(m_end.has_value());
            return m_end.value();
        }

        /**
         * @return FlyString const& 
         */
        FlyString const& filename() const
        {
            return m_filename;
        }

        /**
         * @brief Set the end object
         * 
         * @param end 
         */
        void set_end(Position const& end) 
        { 
            m_end = end; 
        }

        /**
         * @brief Set the parent object
         * 
         * @param parent 
         */
        void set_parent(ASTNode& parent) 
        { 
            m_parent = &parent; 
        }

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const { return {}; }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_identifier() const 
        { 
            return false; 
        }

        virtual bool is_member_expression() const 
        { 
            return false; 
        }

        virtual bool is_variable_or_parameter_declaration() const 
        { 
            return false; 
        }
        
        virtual bool is_function_call() const 
        { 
            return false; 
        }

        virtual bool is_type() const 
        { 
            return false; 
        }

        virtual bool is_declaration() const 
        { 
            return false; 
        }

        virtual bool is_name() const 
        { 
            return false; 
        }

        virtual bool is_dummy_node() const 
        { 
            return false; 
        }

    protected:
        /**
         * @brief Construct a new ASTNode object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        ASTNode(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : m_parent(parent)
            , m_start(start)
            , m_end(end)
            , m_filename(filename)
        {
        }

    private:
        ASTNode* m_parent { nullptr };
        Optional<Position> m_start;
        Optional<Position> m_end;
        FlyString m_filename;
    }; // class ASTNode : public RefCounted<ASTNode> 

    class TranslationUnit : public ASTNode 
    {

    public:
        /**
         * @brief Destroy the TranslationUnit object
         * 
         */
        virtual ~TranslationUnit() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "TranslationUnit"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override { return m_declarations; }

        /**
         * @brief Construct a new Translation Unit object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        TranslationUnit(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : ASTNode(parent, start, end, filename)
        {
        }

        /**
         * @brief Set the declarations object
         * 
         * @param declarations 
         */
        void set_declarations(NonnullRefPtrVector<Declaration>&& declarations) 
        { 
            m_declarations = move(declarations); 
        }

    private:
        NonnullRefPtrVector<Declaration> m_declarations;
    }; // class TranslationUnit : public ASTNode 

    class Statement : public ASTNode 
    {
    public:
        /**
         * @brief Destroy the Statement object
         * 
         */
        virtual ~Statement() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Statement"sv; 
        }

        virtual NonnullRefPtrVector<Declaration> declarations() const override;

    protected:
        /**
         * @brief Construct a new Statement object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Statement(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : ASTNode(parent, start, end, filename)
        {
        }
    }; // class Statement : public ASTNode 

    class Declaration : public Statement 
    {

    public:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_declaration() const override 
        { 
            return true; 
        }

        virtual bool is_variable_declaration() const 
        { 
            return false; 
        }

        virtual bool is_parameter() const 
        { 
            return false; 
        }

        virtual bool is_struct_or_class() const 
        { 
            return false; 
        }

        virtual bool is_struct() const 
        { 
            return false; 
        }

        virtual bool is_class() const 
        { 
            return false; 
        }

        virtual bool is_function() const 
        { 
            return false; 
        }

        virtual bool is_namespace() const 
        { 
            return false; 
        }

        virtual bool is_enum() const 
        { 
            return false; 
        }

        bool is_member() const 
        { 
            return parent() != nullptr && parent()->is_declaration() && verify_cast<Declaration>(parent())->is_struct_or_class(); 
        }

        /**
         * @return Name const* 
         */
        Name const* name() const 
        { 
            return m_name; 
        }
        
        StringView full_name() const;

        /**
         * @brief Set the name object
         * 
         * @param name 
         */
        void set_name(RefPtr<Name> name) 
        { 
            m_name = move(name); 
        }

    protected:
        /**
         * @brief Construct a new Declaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Declaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }

        RefPtr<Name> m_name;
        mutable Optional<String> m_full_name;
    }; // class Declaration : public Statement 

    class InvalidDeclaration : public Declaration 
    {

    public:
        /**
         * @brief Destroy the InvalidDeclaration object
         * 
         */
        virtual ~InvalidDeclaration() override = default;

        virtual StringView class_name() const override 
        { 
            return "InvalidDeclaration"sv; 
        }

        /**
         * @brief Construct a new Invalid Declaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        InvalidDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Declaration(parent, start, end, filename)
        {
        }
    }; // class InvalidDeclaration : public Declaration 

    class FunctionDeclaration : public Declaration 
    {
    public:
        /**
         * @brief Destroy the Function Declaration object
         * 
         */
        virtual ~FunctionDeclaration() override = default;

        virtual StringView class_name() const override 
        { 
            return "FunctionDeclaration"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;
        
        virtual bool is_function() const override 
        { 
            return true; 
        }

        virtual bool is_constructor() const 
        { 
            return false; 
        }

        virtual bool is_destructor() const 
        { 
            return false; 
        }

        /**
         * @return RefPtr<FunctionDefinition> 
         */
        RefPtr<FunctionDefinition> definition() 
        { 
            return m_definition; 
        }

        /**
         * @brief Construct a new Function Declaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        FunctionDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Declaration(parent, start, end, filename)
        {
        }

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override;

        Vector<StringView> const& qualifiers() const 
        { 
            return m_qualifiers; 
        }

        /**
         * @brief Set the qualifiers object
         * 
         * @param qualifiers 
         */
        void set_qualifiers(Vector<StringView> const& qualifiers) 
        { 
            m_qualifiers = qualifiers; 
        }

        /**
         * @return Type const* 
         */
        Type const* return_type() const 
        { 
            return m_return_type.ptr(); 
        }

        /**
         * @brief Set the return type object
         * 
         * @param return_type 
         */
        void set_return_type(RefPtr<Type> const& return_type) 
        { 
            m_return_type = return_type; 
        }

        NonnullRefPtrVector<Parameter> const& parameters() const 
        { 
            return m_parameters; 
        }

        /**
         * @brief Set the parameters object
         * 
         * @param parameters 
         */
        void set_parameters(NonnullRefPtrVector<Parameter> const& parameters) 
        { 
            m_parameters = parameters; 
        }
        
        /**
         * @return FunctionDefinition const* 
         */
        FunctionDefinition const* definition() const 
        { 
            return m_definition.ptr(); 
        }

        /**
         * @brief Set the definition object
         * 
         * @param definition 
         */
        void set_definition(RefPtr<FunctionDefinition>&& definition) 
        { 
            m_definition = move(definition); 
        }

    private:
        Vector<StringView> m_qualifiers;
        RefPtr<Type> m_return_type;
        NonnullRefPtrVector<Parameter> m_parameters;
        RefPtr<FunctionDefinition> m_definition;
    }; // class FunctionDeclaration : public Declaration 

    class VariableOrParameterDeclaration : public Declaration 
    {
    public:
        /**
         * @brief Destroy the VariableOrParameterDeclaration object
         * 
         */
        virtual ~VariableOrParameterDeclaration() override = default;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_variable_or_parameter_declaration() const override 
        { 
            return true; 
        }

        /**
         * @brief Set the type object
         * 
         * @param type 
         */
        void set_type(RefPtr<Type>&& type) 
        { 
            m_type = move(type); 
        }

        /**
         * @return Type const* 
         */
        Type const* type() const 
        { 
            return m_type.ptr(); 
        }

    protected:
        /**
         * @brief Construct a new VariableOrParameterDeclaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        VariableOrParameterDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Declaration(parent, start, end, filename)
        {
        }

        RefPtr<Type> m_type; 
    }; // class VariableOrParameterDeclaration : public Declaration 

    class Parameter : public VariableOrParameterDeclaration 
    {
    public:
        /**
         * @brief Destroy the Parameter object
         * 
         */
        virtual ~Parameter() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Parameter"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_parameter() const override 
        { 
            return true; 
        }

        /**
         * @brief Construct a new Parameter object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         * @param name 
         */
        Parameter(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename, RefPtr<Name> name)
            : VariableOrParameterDeclaration(parent, start, end, filename)
        {
            m_name = name;
        }

        bool is_ellipsis() const 
        { 
            return m_is_ellipsis; 
        }

        /**
         * @brief Set the ellipsis object
         * 
         * @param is_ellipsis 
         */
        void set_ellipsis(bool is_ellipsis) 
        { 
            m_is_ellipsis = is_ellipsis; 
        }

    private:
        bool m_is_ellipsis { false };
    }; // class Parameter : public VariableOrParameterDeclaration 

    class Type : public ASTNode 
    {
    public:
        /**
         * @brief Destroy the Type object
         * 
         */
        virtual ~Type() override = default;
        
        virtual StringView class_name() const override 
        { 
            return "Type"sv; 
        }

        virtual bool is_type() const override 
        { 
            return true; 
        }

        virtual bool is_templatized() const 
        { 
            return false; 
        }

        virtual bool is_named_type() const 
        { 
            return false; 
        }

        virtual String to_string() const = 0;

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        bool is_auto() const 
        { 
            return m_is_auto; 
        }

        /**
         * @brief Set the auto object
         * 
         * @param is_auto 
         */
        void set_auto(bool is_auto) 
        { 
            m_is_auto = is_auto; 
        }

        /**
         * @return Vector<StringView> const& 
         */
        Vector<StringView> const& qualifiers() const 
        { 
            return m_qualifiers; 
        }

        /**
         * @brief Set the qualifiers object
         * 
         * @param qualifiers 
         */
        void set_qualifiers(Vector<StringView>&& qualifiers) 
        { 
            m_qualifiers = move(qualifiers); 
        }

    protected:
        /**
         * @brief Construct a new Type object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Type(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : ASTNode(parent, start, end, filename)
        {
        }

    private:
        bool m_is_auto { false };
        Vector<StringView> m_qualifiers;
    }; // class Type : public ASTNode 

    class NamedType : public Type 
    {
    public:
        /**
         * @brief Destroy the Named Type object
         * 
         */
        virtual ~NamedType() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "NamedType"sv; 
        }

        virtual String to_string() const override;

        virtual bool is_named_type() const override 
        { 
            return true; 
        }

        /**
         * @brief Construct a new NamedType object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        NamedType(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Type(parent, start, end, filename)
        {
        }

        /**
         * @return Name const* 
         */
        Name const* name() const 
        { 
            return m_name.ptr(); 
        }

        /**
         * @brief Set the name object
         * 
         * @param name 
         */
        void set_name(RefPtr<Name>&& name) 
        { 
            m_name = move(name); 
        }

    private:
        RefPtr<Name> m_name;
    }; // class NamedType : public Type 

    class Pointer : public Type 
    {
    public:
        /**
         * @brief Destroy the Pointer object
         * 
         */
        virtual ~Pointer() override = default;

        virtual StringView class_name() const override 
        { 
            return "Pointer"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return String 
         */
        virtual String to_string() const override;

        /**
         * @brief Construct a new Pointer object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Pointer(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Type(parent, start, end, filename)
        {
        }

        /**
         * @return Type const* 
         */
        Type const* pointee() const 
        { 
            return m_pointee.ptr(); 
        }

        /**
         * @brief Set the pointee object
         * 
         * @param pointee 
         */
        void set_pointee(RefPtr<Type>&& pointee) 
        { 
            m_pointee = move(pointee); 
        }

    private:
        RefPtr<Type> m_pointee;
    }; // class Pointer : public Type 

    class Reference : public Type 
    {
    public:
        /**
         * @brief Destroy the Reference object
         * 
         */
        virtual ~Reference() override = default;

        virtual StringView class_name() const override 
        { 
            return "Reference"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        virtual String to_string() const override;

        enum class Kind {
            Lvalue,
            Rvalue,
        }; // enum class Kind

        /**
         * @brief Construct a new Reference object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         * @param kind 
         */
        Reference(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename, Kind kind)
            : Type(parent, start, end, filename)
            , m_kind(kind)
        {
        }

        /**
         * @return Type const* 
         */
        Type const* referenced_type() const 
        { 
            return m_referenced_type.ptr(); 
        }

        /**
         * @brief Set the referenced type object
         * 
         * @param pointee 
         */
        void set_referenced_type(RefPtr<Type>&& pointee) 
        { 
            m_referenced_type = move(pointee); 
        }

        /**
         * @return Kind 
         */
        Kind kind() const 
        { 
            return m_kind; 
        }

    private:
        RefPtr<Type const> m_referenced_type;
        Kind m_kind;
    }; // class Reference : public Type 

    class FunctionType : public Type 
    {
    public:
        /**
         * @brief Destroy the Function Type object
         * 
         */
        virtual ~FunctionType() override = default;

        virtual StringView class_name() const override 
        { 
            return "FunctionType"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        virtual String to_string() const override;

        /**
         * @brief Construct a new FunctionType object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        FunctionType(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Type(parent, start, end, filename)
        {
        }

        /**
         * @brief Set the return type object
         * 
         * @param type 
         */
        void set_return_type(Type& type) 
        { 
            m_return_type = type; 
        }

        /**
         * @brief Set the parameters object
         * 
         * @param parameters 
         */
        void set_parameters(NonnullRefPtrVector<Parameter> parameters) 
        { 
            m_parameters = move(parameters); 
        }

    private:
        RefPtr<Type> m_return_type;
        NonnullRefPtrVector<Parameter> m_parameters;
    }; // class FunctionType : public Type 

    class FunctionDefinition : public ASTNode 
    {
    public:
        /**
         * @brief Destroy the Function Definition object
         * 
         */
        virtual ~FunctionDefinition() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "FunctionDefinition"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Construct a new FunctionDefinition object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        FunctionDefinition(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : ASTNode(parent, start, end, filename)
        {
        }

        virtual NonnullRefPtrVector<Declaration> declarations() const override;

        NonnullRefPtrVector<Statement> const& statements() 
        { 
            return m_statements; 
        }

        /**
         * @param statement 
         */
        void add_statement(NonnullRefPtr<Statement>&& statement) 
        { 
            m_statements.append(move(statement)); 
        }

    private:
        NonnullRefPtrVector<Statement> m_statements;
    }; // class FunctionDefinition : public ASTNode 

    class InvalidStatement : public Statement 
    {
    public:
        /**
         * @brief Destroy the Invalid Statement object
         * 
         */
        virtual ~InvalidStatement() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "InvalidStatement"sv; 
        }

        /**
         * @brief Construct a new InvalidStatement object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        InvalidStatement(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }
    }; // class InvalidStatement : public Statement 

    class Expression : public Statement 
    {
    public:
        /**
         * @brief Destroy the Expression object
         * 
         */
        virtual ~Expression() override = default;
        
        virtual StringView class_name() const override 
        { 
            return "Expression"sv; 
        }

    protected:
        /**
         * @brief Construct a new Expression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Expression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }
    }; // class Expression : public Statement 

    class InvalidExpression : public Expression 
    {
    public:
        /**
         * @brief Destroy the InvalidExpression object
         * 
         */
        virtual ~InvalidExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "InvalidExpression"sv; 
        }

        /**
         * @brief Construct a new InvalidExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        InvalidExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }
    }; // class InvalidExpression : public Expression 

    class VariableDeclaration : public VariableOrParameterDeclaration 
    {
    public:
        /**
         * @brief Destroy the VariableDeclaration object
         * 
         */
        virtual ~VariableDeclaration() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "VariableDeclaration"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Construct a new VariableDeclaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        VariableDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : VariableOrParameterDeclaration(parent, start, end, filename)
        {
        }

        virtual bool is_variable_declaration() const override 
        { 
            return true; 
        }

        /**
         * @return Expression const* 
         */
        Expression const* initial_value() const 
        { 
            return m_initial_value; 
        }

        /**
         * @brief Set the initial value object
         * 
         * @param initial_value 
         */
        void set_initial_value(RefPtr<Expression>&& initial_value) 
        { 
            m_initial_value = move(initial_value); 
        }

    private:
        RefPtr<Expression> m_initial_value;
    }; // class VariableDeclaration : public VariableOrParameterDeclaration 

    class Identifier : public Expression 
    {
    public:
        /**
         * @brief Destroy the Identifier object
         * 
         */
        virtual ~Identifier() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Identifier"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Construct a new Identifier object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         * @param name 
         */
        Identifier(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename, StringView name)
            : Expression(parent, start, end, filename)
            , m_name(name)
        {
        }

        /**
         * @brief Construct a new Identifier object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Identifier(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Identifier(parent, start, end, filename, {})
        {
        }

        virtual bool is_identifier() const override 
        { 
            return true; 
        }

        /**
         * @return StringView 
         */
        StringView name() const 
        { 
            return m_name; 
        }

        /**
         * @brief Set the name object
         * 
         * @param name 
         */
        void set_name(StringView&& name) 
        { 
            m_name = move(name); 
        }

    private:
        StringView m_name;
    }; // class Identifier : public Expression 

    class Name : public Expression 
    {
    public:
        /**
         * @brief Destroy the Name object
         * 
         */
        virtual ~Name() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Name"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_name() const override 
        { 
            return true; 
        }

        virtual bool is_templatized() const 
        { 
            return false; 
        }

        /**
         * @brief Construct a new Name object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Name(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @return StringView 
         */
        virtual StringView full_name() const;

        /**
         * @return Identifier const* 
         */
        Identifier const* name() const 
        { 
            return m_name.ptr(); 
        }

        /**
         * @brief Set the name object
         * 
         * @param name 
         */
        void set_name(RefPtr<Identifier>&& name) 
        { 
            m_name = move(name); 
        }

        /**
         * @return NonnullRefPtrVector<Identifier> const& 
         */
        NonnullRefPtrVector<Identifier> const& scope() const 
        { 
            return m_scope; 
        }

        /**
         * @brief Set the scope object
         * 
         * @param scope 
         */
        void set_scope(NonnullRefPtrVector<Identifier> scope) 
        { 
            m_scope = move(scope); 
        }

        /**
         * @param part 
         */
        void add_to_scope(NonnullRefPtr<Identifier>&& part) 
        { 
            m_scope.append(move(part)); 
        }

    private:
        RefPtr<Identifier> m_name;
        NonnullRefPtrVector<Identifier> m_scope;
        mutable Optional<String> m_full_name;
    }; // class Name : public Expression 

    class TemplatizedName : public Name 
    {
    public:
        /**
         * @brief Destroy the TemplatizedName object
         * 
         */
        virtual ~TemplatizedName() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "TemplatizedName"sv; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_templatized() const override 
        { 
            return true; 
        }

        /**
         * @return StringView 
         */
        virtual StringView full_name() const override;

        /**
         * @brief Construct a new TemplatizedName object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        TemplatizedName(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Name(parent, start, end, filename)
        {
        }

        /**
         * @param type 
         */
        void add_template_argument(NonnullRefPtr<Type>&& type) 
        { 
            m_template_arguments.append(move(type)); 
        }

    private:
        NonnullRefPtrVector<Type> m_template_arguments;
        mutable Optional<String> m_full_name;
    }; // class TemplatizedName : public Name 

    class NumericLiteral : public Expression 
    {
    public:
        /**
         * @brief Destroy the NumericLiteral object
         * 
         */
        virtual ~NumericLiteral() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "NumericLiteral"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Construct a new NumericLiteral object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         * @param value 
         */
        NumericLiteral(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename, StringView value)
            : Expression(parent, start, end, filename)
            , m_value(value)
        {
        }

    private:
        StringView m_value;
    }; // class NumericLiteral : public Expression 

    class NullPointerLiteral : public Expression 
    {
    public:
        /**
         * @brief Destroy the NullPointerLiteral object
         * 
         */
        virtual ~NullPointerLiteral() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "NullPointerLiteral"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Construct a new NullPointerLiteral object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        NullPointerLiteral(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }
    }; // class NullPointerLiteral : public Expression 

    class BooleanLiteral : public Expression 
    {
    public:
        /**
         * @brief Destroy the Boolean Literal object
         * 
         */
        virtual ~BooleanLiteral() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "BooleanLiteral"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Construct a new BooleanLiteral object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         * @param value 
         */
        BooleanLiteral(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename, bool value)
            : Expression(parent, start, end, filename)
            , m_value(value)
        {
        }

    private:
        bool m_value;
    }; // class BooleanLiteral : public Expression 

    enum class BinaryOp {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        GreaterThan,
        GreaterThanEquals,
        LessThan,
        LessThanEquals,
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        LeftShift,
        RightShift,
        EqualsEquals,
        NotEqual,
        LogicalOr,
        LogicalAnd,
        Arrow,
    }; // enum class BinaryOp

    class BinaryExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new BinaryExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        BinaryExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the BinaryExpression object
         * 
         */
        virtual ~BinaryExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "BinaryExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return BinaryOp 
         */
        BinaryOp op() const 
        { 
            return m_op; 
        }

        /**
         * @brief Set the op object
         * 
         * @param op 
         */
        void set_op(BinaryOp op) 
        { 
            m_op = op; 
        }
        
        /**
         * @return Expression const* 
         */
        Expression const* lhs() const 
        { 
            return m_lhs.ptr(); 
        }

        /**
         * @brief Set the lhs object
         * 
         * @param e 
         */
        void set_lhs(RefPtr<Expression>&& e) 
        { 
            m_lhs = move(e); 
        }

        /**
         * @return Expression const* 
         */
        Expression const* rhs() const 
        { 
            return m_rhs.ptr(); 
        }

        /**
         * @brief Set the rhs object
         * 
         * @param e 
         */
        void set_rhs(RefPtr<Expression>&& e) 
        { 
            m_rhs = move(e); 
        }

    private:
        BinaryOp m_op;
        RefPtr<Expression> m_lhs;
        RefPtr<Expression> m_rhs;
    }; // class BinaryExpression : public Expression 

    enum class AssignmentOp {
        Assignment,
        AdditionAssignment,
        SubtractionAssignment,
    }; // enum class AssignmentOp

    class AssignmentExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new AssignmentExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        AssignmentExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the AssignmentExpression object
         * 
         */
        virtual ~AssignmentExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "AssignmentExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return AssignmentOp 
         */
        AssignmentOp op() const 
        { 
            return m_op; 
        }

        /**
         * @brief Set the op object
         * 
         * @param op 
         */
        void set_op(AssignmentOp op) 
        { 
            m_op = op; 
        }

        /**
         * @return Expression const* 
         */
        Expression const* lhs() const 
        { 
            return m_lhs; 
        }

        /**
         * @brief Set the lhs object
         * 
         * @param e 
         */
        void set_lhs(RefPtr<Expression>&& e) 
        { 
            m_lhs = move(e); 
        }

        /**
         * @return Expression const* 
         */
        Expression const* rhs() const 
        { 
            return m_rhs; 
        }

        /**
         * @brief Set the rhs object
         * 
         * @param e 
         */
        void set_rhs(RefPtr<Expression>&& e) 
        { 
            m_rhs = move(e); 
        }

    private:
        AssignmentOp m_op {};
        RefPtr<Expression> m_lhs;
        RefPtr<Expression> m_rhs;
    }; // class AssignmentExpression : public Expression 

    class FunctionCall : public Expression 
    {
    public:
        /**
         * @brief Construct a new FunctionCall object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        FunctionCall(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the Function Call object
         * 
         */
        virtual ~FunctionCall() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "FunctionCall"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_function_call() const override 
        { 
            return true; 
        }

        /**
         * @return Expression const* 
         */
        Expression const* callee() const 
        { 
            return m_callee.ptr(); 
        }

        /**
         * @brief Set the callee object
         * 
         * @param callee 
         */
        void set_callee(RefPtr<Expression>&& callee) 
        { 
            m_callee = move(callee); 
        }

        /**
         * @param arg 
         */
        void add_argument(NonnullRefPtr<Expression>&& arg) 
        { 
            m_arguments.append(move(arg)); 
        }

        /**
         * @return NonnullRefPtrVector<Expression> const& 
         */
        NonnullRefPtrVector<Expression> const& arguments() const 
        { 
            return m_arguments; 
        }

    private:
        RefPtr<Expression> m_callee;
        NonnullRefPtrVector<Expression> m_arguments;
    }; // class FunctionCall : public Expression 

    class StringLiteral final : public Expression 
    {
    public:
        /**
         * @brief Construct a new StringLiteral object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        StringLiteral(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the StringLiteral object
         * 
         */
        ~StringLiteral() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "StringLiteral"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return String const& 
         */
        String const& value() const 
        { 
            return m_value; 
        }

        /**
         * @brief Set the value object
         * 
         * @param value 
         */
        void set_value(String value) 
        { 
            m_value = move(value); 
        }

    private:
        String m_value;
    }; // class StringLiteral final : public Expression 

    class ReturnStatement : public Statement 
    {
    public:
        /**
         * @brief Destroy the ReturnStatement object
         * 
         */
        virtual ~ReturnStatement() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "ReturnStatement"sv; 
        }

        /**
         * @brief Construct a new ReturnStatement object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        ReturnStatement(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }
        
        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return Expression const* 
         */
        Expression const* value() const 
        { 
            return m_value.ptr(); 
        }

        /**
         * @brief Set the value object
         * 
         * @param value 
         */
        void set_value(RefPtr<Expression>&& value) 
        { 
            m_value = move(value); 
        }

    private:
        RefPtr<Expression> m_value;
    }; // class ReturnStatement : public Statement 

    class EnumDeclaration : public Declaration 
    {
    public:
        /**
         * @brief Destroy the EnumDeclaration object
         * 
         */
        virtual ~EnumDeclaration() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "EnumDeclaration"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_enum() const override 
        { 
            return true; 
        }

        /**
         * @brief Construct a new EnumDeclaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        EnumDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Declaration(parent, start, end, filename)
        {
        }

        enum class Type {
            RegularEnum,
            EnumClass
        }; // enum class Type

        /**
         * @brief Set the type object
         * 
         * @param type 
         */
        void set_type(Type type) 
        { 
            m_type = type; 
        }

        /**
         * @param entry 
         * @param value 
         */
        void add_entry(StringView entry, RefPtr<Expression> value = nullptr) 
        { 
            m_entries.append({ entry, move(value) }); 
        }

    private:
        Type m_type { Type::RegularEnum };

        struct EnumerationEntry {
            StringView name;
            RefPtr<Expression> value;
        }; // struct EnumerationEntry

        Vector<EnumerationEntry> m_entries;
    }; // class EnumDeclaration : public Declaration 

    class StructOrClassDeclaration : public Declaration 
    {
    public:
        /**
         * @brief Destroy the StructOrClassDeclaration object
         * 
         */
        virtual ~StructOrClassDeclaration() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "StructOrClassDeclaration"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_struct_or_class() const override 
        { 
            return true; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_struct() const override 
        { 
            return m_type == Type::Struct; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_class() const override 
        { 
            return m_type == Type::Class; 
        }

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override;

        enum class Type {
            Struct,
            Class
        }; // enum class Type

        /**
         * @brief Construct a new StructOrClassDeclaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         * @param type 
         */
        StructOrClassDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename, StructOrClassDeclaration::Type type)
            : Declaration(parent, start, end, filename)
            , m_type(type)
        {
        }

        /**
         * @return NonnullRefPtrVector<Declaration> const& 
         */
        NonnullRefPtrVector<Declaration> const& members() const 
        { 
            return m_members; 
        }

        /**
         * @brief Set the members object
         * 
         * @param members 
         */
        void set_members(NonnullRefPtrVector<Declaration>&& members) 
        { 
            m_members = move(members); 
        }

        /**
         * @return NonnullRefPtrVector<Name> const& 
         */
        NonnullRefPtrVector<Name> const& baseclasses() const 
        { 
            return m_baseclasses; 
        }

        /**
         * @brief Set the baseclasses object
         * 
         * @param baseclasses 
         */
        void set_baseclasses(NonnullRefPtrVector<Name>&& baseclasses) 
        { 
            m_baseclasses = move(baseclasses); 
        }

    private:
        StructOrClassDeclaration::Type m_type;
        NonnullRefPtrVector<Declaration> m_members;
        NonnullRefPtrVector<Name> m_baseclasses;
    }; // class StructOrClassDeclaration : public Declaration 

    enum class UnaryOp {
        Invalid,
        BitwiseNot,
        Not,
        Plus,
        Minus,
        PlusPlus,
        Address, 
    }; // enum class UnaryOp

    class UnaryExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new UnaryExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        UnaryExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the UnaryExpression object
         * 
         */
        virtual ~UnaryExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "UnaryExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Set the op object
         * 
         * @param op 
         */
        void set_op(UnaryOp op) 
        { 
            m_op = op; 
        }

        /**
         * @brief Set the lhs object
         * 
         * @param e 
         */
        void set_lhs(RefPtr<Expression>&& e) 
        { 
            m_lhs = move(e); 
        }

    private:
        UnaryOp m_op;
        RefPtr<Expression> m_lhs;
    }; // class UnaryExpression : public Expression 

    class MemberExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new MemberExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        MemberExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the MemberExpression object
         * 
         */
        virtual ~MemberExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "MemberExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_member_expression() const override 
        { 
            return true; 
        }

        /**
         * @return Expression const* 
         */
        Expression const* object() const 
        { 
            return m_object.ptr(); 
        }

        /**
         * @brief Set the object object
         * 
         * @param object 
         */
        void set_object(RefPtr<Expression>&& object) 
        { 
            m_object = move(object); 
        }

        /**
         * @return Expression const* 
         */
        Expression const* property() const 
        { 
            return m_property.ptr(); 
        }

        /**
         * @brief Set the property object
         * 
         * @param property 
         */
        void set_property(RefPtr<Expression>&& property) 
        { 
            m_property = move(property); 
        }

    private:
        RefPtr<Expression> m_object;
        RefPtr<Expression> m_property;
    }; // class MemberExpression : public Expression 

    class ForStatement : public Statement 
    {
    public:
        /**
         * @brief Construct a new For Statement object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        ForStatement(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the ForStatement object
         * 
         */
        virtual ~ForStatement() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "ForStatement"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override;


        /**
         * @brief Set the init object
         * 
         * @param init 
         */
        void set_init(RefPtr<VariableDeclaration>&& init) 
        { 
            m_init = move(init); 
        }

        /**
         * @brief Set the test object
         * 
         * @param test 
         */
        void set_test(RefPtr<Expression>&& test) 
        { 
            m_test = move(test); 
        }

        /**
         * @brief Set the update object
         * 
         * @param update 
         */
        void set_update(RefPtr<Expression>&& update) 
        { 
            m_update = move(update); 
        }

        /**
         * @brief Set the body object
         * 
         * @param body 
         */
        void set_body(RefPtr<Statement>&& body) 
        { 
            m_body = move(body); 
        }

        /**
         * @return Statement const* 
         */
        Statement const* body() const 
        { 
            return m_body.ptr(); 
        }

    private:
        RefPtr<VariableDeclaration> m_init;
        RefPtr<Expression> m_test;
        RefPtr<Expression> m_update;
        RefPtr<Statement> m_body;
    }; // class ForStatement : public Statement 

    class BlockStatement final : public Statement
    {
    public:

        /**
         * @brief Construct a new BlockStatement object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        BlockStatement(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the BlockStatement object
         * 
         */
        virtual ~BlockStatement() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "BlockStatement"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override;

        /**
         * @param statement 
         */
        void add_statement(NonnullRefPtr<Statement>&& statement) 
        { 
            m_statements.append(move(statement)); 
        }

    private:
        NonnullRefPtrVector<Statement> m_statements;
    }; // class BlockStatement final : public Statement

    class Comment final : public Statement 
    {
    public:

        /**
         * @brief Construct a new Comment object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Comment(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the Comment object
         * 
         */
        virtual ~Comment() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Comment"sv; 
        }
    }; // class Comment final : public Statement

    class IfStatement : public Statement 
    {
    public:
        /**
         * @brief Construct a new IfStatement object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        IfStatement(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Statement(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the IfStatement object
         * 
         */
        virtual ~IfStatement() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "IfStatement"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override;

        /**
         * @brief Set the predicate object
         * 
         * @param predicate 
         */
        void set_predicate(RefPtr<Expression>&& predicate) 
        { 
            m_predicate = move(predicate); 
        }

        /**
         * @brief Set the then statement object
         * 
         * @param then 
         */
        void set_then_statement(RefPtr<Statement>&& then) 
        { 
            m_then = move(then); 
        }

        /**
         * @brief Set the else statement object
         * 
         * @param _else 
         */
        void set_else_statement(RefPtr<Statement>&& _else) 
        { 
            m_else = move(_else); 
        }

        /**
         * @return Statement const* 
         */
        Statement const* then_statement() const 
        { 
            return m_then.ptr(); 
        }

        Statement const* else_statement() const 
        { 
            return m_else.ptr(); 
        }

    private:
        RefPtr<Expression> m_predicate;
        RefPtr<Statement> m_then;
        RefPtr<Statement> m_else;
    }; // class IfStatement : public Statement

    class NamespaceDeclaration : public Declaration 
    {
    public:
        /**
         * @brief Destroy the NamespaceDeclaration object
         * 
         */
        virtual ~NamespaceDeclaration() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "NamespaceDeclaration"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_namespace() const override 
        {
            return true; 
        }

        /**
         * @brief Construct a new NamespaceDeclaration object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        NamespaceDeclaration(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Declaration(parent, start, end, filename)
        {
        }

        /**
         * @return NonnullRefPtrVector<Declaration> 
         */
        virtual NonnullRefPtrVector<Declaration> declarations() const override 
        { 
            return m_declarations; 
        }

        /** 
         * @param declaration 
         */
        void add_declaration(NonnullRefPtr<Declaration>&& declaration) 
        { 
            m_declarations.append(move(declaration)); 
        }

    private:
        NonnullRefPtrVector<Declaration> m_declarations;
    }; // class NamespaceDeclaration : public Declaration

    class CppCastExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new CppCastExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        CppCastExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the CppCastExpression object
         * 
         */
        virtual ~CppCastExpression() override = default;

        /** 
         * @return StringView 
         */
        virtual StringView class_name() const override
        { 
            return "CppCastExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Set the cast type object
         *
         * @param cast_type 
         */
        void set_cast_type(StringView cast_type) 
        { 
            m_cast_type = move(cast_type); 
        }

        /**
         * @brief Set the type object
         *
         * @param type 
         */
        void set_type(NonnullRefPtr<Type>&& type) 
        { 
            m_type = move(type); 
        }
 
        /**
         * @brief Set the expression object
         * 
         * @param e 
         */
        void set_expression(NonnullRefPtr<Expression>&& e) 
        { 
            m_expression = move(e); 
        }

    private:
        StringView m_cast_type;
        RefPtr<Type> m_type;
        RefPtr<Expression> m_expression;
    }; // class CppCastExpression : public Expression

    class CStyleCastExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new CStyleCastExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        CStyleCastExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the CStyleCastExpression object
         * 
         */
        virtual ~CStyleCastExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "CStyleCastExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Set the type object
         * 
         * @param type 
         */
        void set_type(NonnullRefPtr<Type>&& type) 
        { 
            m_type = move(type); 
        }

        /**
         * @brief Set the expression object
         * 
         * @param e 
         */
        void set_expression(NonnullRefPtr<Expression>&& e) 
        { 
            m_expression = move(e); 
        }

    private:
        RefPtr<Type> m_type;
        RefPtr<Expression> m_expression;
    }; // class CStyleCastExpression : public Expression

    class SizeofExpression : public Expression 
    {
    public:
        /**
         * @brief Construct a new SizeofExpression object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        SizeofExpression(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the SizeofExpression object
         * 
         */
        virtual ~SizeofExpression() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "SizeofExpression"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @brief Set the type object
         * 
         * @param type 
         */
        void set_type(RefPtr<Type>&& type) { m_type = move(type); }

    private:
        RefPtr<Type> m_type;
    }; // class SizeofExpression : public Expression

    class BracedInitList : public Expression 
    {
    public:
        /**
         * @brief Construct a new BracedInitList object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        BracedInitList(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : Expression(parent, start, end, filename)
        {
        }

        /**
         * @brief Destroy the BracedInitList object
         * 
         */
        virtual ~BracedInitList() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "BracedInitList"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @param exp 
         */
        void add_expression(NonnullRefPtr<Expression>&& exp) { m_expressions.append(move(exp)); }

    private:
        NonnullRefPtrVector<Expression> m_expressions;
    }; // class BracedInitList : public Expression

    class DummyAstNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new DummyAstNode object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        DummyAstNode(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : ASTNode(parent, start, end, filename)
        {
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_dummy_node() const override 
        { 
            return true; 
        }

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override
        { 
            return "DummyAstNode"sv; 
        }

        virtual void dump(FILE* = stdout, size_t = 0) const override { }
    }; // class DummyAstNode : public ASTNode 

    class Constructor : public FunctionDeclaration 
    {
    public:
        /**
         * @brief Destroy the Constructor object
         * 
         */
        virtual ~Constructor() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Constructor"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_constructor() const override 
        { 
            return true;
        }

        /**
         * @brief Construct a new Constructor object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Constructor(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : FunctionDeclaration(parent, start, end, filename)
        {
        }
    }; // class Constructor : public FunctionDeclaration

    class Destructor : public FunctionDeclaration 
    {
    public:
        /**
         * @brief Destroy the Destructor object
         * 
         */
        virtual ~Destructor() override = default;

        /**
         * @return StringView 
         */
        virtual StringView class_name() const override 
        { 
            return "Destructor"sv; 
        }

        /**
         * @param indent 
         */
        virtual void dump(FILE* = stdout, size_t indent = 0) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_destructor() const override 
        { 
            return true; 
        }

        /**
         * @brief Construct a new Destructor object
         * 
         * @param parent 
         * @param start 
         * @param end 
         * @param filename 
         */
        Destructor(ASTNode* parent, Optional<Position> start, Optional<Position> end, String const& filename)
            : FunctionDeclaration(parent, start, end, filename)
        {
        }
    }; // class Destructor : public FunctionDeclaration

    /**
     * @tparam  
     * @return true 
     * @return false 
     */
    template<>
    inline bool ASTNode::fast_is<Identifier>() const { return is_identifier(); }
    template<>
    inline bool ASTNode::fast_is<MemberExpression>() const { return is_member_expression(); }
    template<>
    inline bool ASTNode::fast_is<VariableOrParameterDeclaration>() const { return is_variable_or_parameter_declaration(); }
    template<>
    inline bool ASTNode::fast_is<FunctionCall>() const { return is_function_call(); }
    template<>
    inline bool ASTNode::fast_is<Type>() const { return is_type(); }
    template<>
    inline bool ASTNode::fast_is<Declaration>() const { return is_declaration(); }
    template<>
    inline bool ASTNode::fast_is<Name>() const { return is_name(); }
    template<>
    inline bool ASTNode::fast_is<DummyAstNode>() const { return is_dummy_node(); }

    template<>
    inline bool ASTNode::fast_is<VariableDeclaration>() const { return is_declaration() && verify_cast<Declaration>(*this).is_variable_declaration(); }
    template<>
    inline bool ASTNode::fast_is<StructOrClassDeclaration>() const { return is_declaration() && verify_cast<Declaration>(*this).is_struct_or_class(); }
    template<>
    inline bool ASTNode::fast_is<FunctionDeclaration>() const { return is_declaration() && verify_cast<Declaration>(*this).is_function(); }
    template<>
    inline bool ASTNode::fast_is<NamespaceDeclaration>() const { return is_declaration() && verify_cast<Declaration>(*this).is_namespace(); }
    template<>
    inline bool ASTNode::fast_is<Constructor>() const { return is_declaration() && verify_cast<Declaration>(*this).is_function() && verify_cast<FunctionDeclaration>(*this).is_constructor(); }
    template<>
    inline bool ASTNode::fast_is<Destructor>() const { return is_declaration() && verify_cast<Declaration>(*this).is_function() && verify_cast<FunctionDeclaration>(*this).is_destructor(); }
    template<>
    inline bool ASTNode::fast_is<NamedType>() const { return is_type() && verify_cast<Type>(*this).is_named_type(); }
    template<>
    inline bool ASTNode::fast_is<TemplatizedName>() const { return is_name() && verify_cast<Name>(*this).is_templatized(); }

} // namespace Cpp
