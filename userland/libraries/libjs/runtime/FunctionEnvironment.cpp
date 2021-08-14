/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/Interpreter.h>
#include <libjs/runtime/FunctionEnvironment.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

FunctionEnvironment::FunctionEnvironment(Environment* parent_scope, HashMap<FlyString, Variable> variables)
    : DeclarativeEnvironment(variables, parent_scope)
{
}

FunctionEnvironment::~FunctionEnvironment()
{
}

void FunctionEnvironment::visit_edges(Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(m_this_value);
    visitor.visit(m_new_target);
    visitor.visit(m_function_object);
}

Value FunctionEnvironment::get_super_base() const
{
    VERIFY(m_function_object);
    auto home_object = m_function_object->home_object();
    if (!home_object)
        return js_undefined();
    return home_object->internal_get_prototype_of();
}


bool FunctionEnvironment::has_this_binding() const
{
    if (this_binding_status() == ThisBindingStatus::Lexical)
        return false;
    return true;
}

bool FunctionEnvironment::has_super_binding() const
{
    if (this_binding_status() == ThisBindingStatus::Lexical)
        return false;
    if (!function_object().home_object())
        return false;
    return true;
}

Value FunctionEnvironment::get_this_binding(GlobalObject& global_object) const
{
    VERIFY(has_this_binding());
    if (this_binding_status() == ThisBindingStatus::Uninitialized) {
        vm().throw_exception<ReferenceError>(global_object, ErrorType::ThisHasNotBeenInitialized);
        return {};
    }
    return m_this_value;
}

Value FunctionEnvironment::bind_this_value(GlobalObject& global_object, Value this_value)
{
    VERIFY(this_binding_status() != ThisBindingStatus::Lexical);
    if (this_binding_status() == ThisBindingStatus::Initialized) {
        vm().throw_exception<ReferenceError>(global_object, ErrorType::ThisIsAlreadyInitialized);
        return {};
    }
    m_this_value = this_value;
    m_this_binding_status = ThisBindingStatus::Initialized;
    return this_value;
}

}
