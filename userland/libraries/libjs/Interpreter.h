/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <base/HashMap.h>
#include <base/String.h>
#include <base/Vector.h>
#include <base/Weakable.h>
#include <libjs/AST.h>
#include <libjs/Forward.h>
#include <libjs/heap/DeferGC.h>
#include <libjs/heap/Heap.h>
#include <libjs/runtime/DeclarativeEnvironment.h>
#include <libjs/runtime/ErrorTypes.h>
#include <libjs/runtime/Exception.h>
#include <libjs/runtime/MarkedValueList.h>
#include <libjs/runtime/VM.h>
#include <libjs/runtime/Value.h>

namespace JS {

struct ExecutingASTNodeChain {
    ExecutingASTNodeChain* previous { nullptr };
    const ASTNode& node;
};

class Interpreter : public Weakable<Interpreter> {
public:
    template<typename GlobalObjectType, typename... Args>
    static NonnullOwnPtr<Interpreter> create(VM& vm, Args&&... args)
    {
        DeferGC defer_gc(vm.heap());
        auto interpreter = adopt_own(*new Interpreter(vm));
        VM::InterpreterExecutionScope scope(*interpreter);
        interpreter->m_global_object = make_handle(static_cast<Object*>(interpreter->heap().allocate_without_global_object<GlobalObjectType>(forward<Args>(args)...)));
        static_cast<GlobalObjectType*>(interpreter->m_global_object.cell())->initialize_global_object();
        return interpreter;
    }

    static NonnullOwnPtr<Interpreter> create_with_existing_global_object(GlobalObject&);

    ~Interpreter();

    void run(GlobalObject&, const Program&);

    GlobalObject& global_object();
    const GlobalObject& global_object() const;

    ALWAYS_INLINE VM& vm() { return *m_vm; }
    ALWAYS_INLINE const VM& vm() const { return *m_vm; }
    ALWAYS_INLINE Heap& heap() { return vm().heap(); }
    ALWAYS_INLINE Exception* exception() { return vm().exception(); }

    Environment* lexical_environment() { return vm().lexical_environment(); }

    FunctionEnvironment* current_function_environment();

    void enter_scope(const ScopeNode&, ScopeType, GlobalObject&);
    void exit_scope(const ScopeNode&);

    void push_ast_node(ExecutingASTNodeChain& chain_node)
    {
        chain_node.previous = m_ast_node_chain;
        m_ast_node_chain = &chain_node;
    }

    void pop_ast_node()
    {
        VERIFY(m_ast_node_chain);
        m_ast_node_chain = m_ast_node_chain->previous;
    }

    const ASTNode* current_node() const { return m_ast_node_chain ? &m_ast_node_chain->node : nullptr; }
    ExecutingASTNodeChain* executing_ast_node_chain() { return m_ast_node_chain; }
    const ExecutingASTNodeChain* executing_ast_node_chain() const { return m_ast_node_chain; }

    Value execute_statement(GlobalObject&, const Statement&, ScopeType = ScopeType::Block);

private:
    explicit Interpreter(VM&);

    void push_scope(ScopeFrame frame);

    Vector<ScopeFrame> m_scope_stack;
    ExecutingASTNodeChain* m_ast_node_chain { nullptr };

    NonnullRefPtr<VM> m_vm;

    Handle<Object> m_global_object;
};

}
