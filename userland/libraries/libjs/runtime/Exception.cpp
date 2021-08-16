/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/String.h>
#include <libjs/AST.h>
#include <libjs/Interpreter.h>
#include <libjs/runtime/Exception.h>
#include <libjs/runtime/VM.h>

namespace JS {

Exception::Exception(Value value)
    : m_value(value)
{
    auto& vm = this->vm();
    m_traceback.ensure_capacity(vm.execution_context_stack().size());
    for (ssize_t i = vm.execution_context_stack().size() - 1; i >= 0; i--) {
        auto* context = vm.execution_context_stack()[i];
        auto function_name = context->function_name;
        if (function_name.is_empty())
            function_name = "<anonymous>";
        m_traceback.empend(
            move(function_name),
            context->current_node ? context->current_node->source_range() : SourceRange {});
    }
}

void Exception::visit_edges(Visitor& visitor)
{
    Cell::visit_edges(visitor);
    visitor.visit(m_value);
}

}
