/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/Interpreter.h>
#include <libjs/runtime/DeclarativeEnvironment.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Value.h>

namespace JS {

DeclarativeEnvironment::DeclarativeEnvironment()
    : Environment(nullptr)
{
}

DeclarativeEnvironment::DeclarativeEnvironment(Environment* parent_scope)
    : Environment(parent_scope)
{
}

DeclarativeEnvironment::DeclarativeEnvironment(HashMap<FlyString, Variable> variables, Environment* parent_scope)
    : Environment(parent_scope)
    , m_variables(move(variables))
{
}

DeclarativeEnvironment::~DeclarativeEnvironment()
{
}

void DeclarativeEnvironment::visit_edges(Visitor& visitor)
{
    Base::visit_edges(visitor);
    for (auto& it : m_variables)
        visitor.visit(it.value.value);
    for (auto& it : m_bindings)
        visitor.visit(it.value.value);
}

Optional<Variable> DeclarativeEnvironment::get_from_environment(FlyString const& name) const
{
    return m_variables.get(name);
}

bool DeclarativeEnvironment::put_into_environment(FlyString const& name, Variable variable)
{
    m_variables.set(name, variable);
    return true;
}

bool DeclarativeEnvironment::delete_from_environment(FlyString const& name)
{
    return m_variables.remove(name);
}

bool DeclarativeEnvironment::has_binding(FlyString const& name) const
{
    return m_bindings.contains(name);
}

void DeclarativeEnvironment::create_mutable_binding(GlobalObject&, FlyString const& name, bool can_be_deleted)
{
    auto result = m_bindings.set(name,
        Binding {
            .value = {},
            .strict = false,
            .mutable_ = true,
            .can_be_deleted = can_be_deleted,
            .initialized = false,
        });
    VERIFY(result == Base::HashSetResult::InsertedNewEntry);
}

void DeclarativeEnvironment::create_immutable_binding(GlobalObject&, FlyString const& name, bool strict)
{
    auto result = m_bindings.set(name,
        Binding {
            .value = {},
            .strict = strict,
            .mutable_ = false,
            .can_be_deleted = false,
            .initialized = false,
        });
    VERIFY(result == Base::HashSetResult::InsertedNewEntry);
}

void DeclarativeEnvironment::initialize_binding(GlobalObject&, FlyString const& name, Value value)
{
    auto it = m_bindings.find(name);
    VERIFY(it != m_bindings.end());
    VERIFY(it->value.initialized == false);
    it->value.value = value;
    it->value.initialized = true;
}

void DeclarativeEnvironment::set_mutable_binding(GlobalObject& global_object, FlyString const& name, Value value, bool strict)
{
    auto it = m_bindings.find(name);
    if (it == m_bindings.end()) {
        if (strict) {
            global_object.vm().throw_exception<ReferenceError>(global_object, ErrorType::UnknownIdentifier, name);
            return;
        }
        create_mutable_binding(global_object, name, true);
        initialize_binding(global_object, name, value);
        return;
    }

    if (it->value.strict)
        strict = true;

    if (!it->value.initialized) {
        global_object.vm().throw_exception<ReferenceError>(global_object, ErrorType::BindingNotInitialized, name);
        return;
    }

    if (it->value.mutable_) {
        it->value.value = value;
    } else {
        if (strict) {
            global_object.vm().throw_exception<TypeError>(global_object, ErrorType::InvalidAssignToConst);
        }
    }
}

Value DeclarativeEnvironment::get_binding_value(GlobalObject& global_object, FlyString const& name, bool)
{
    auto it = m_bindings.find(name);
    VERIFY(it != m_bindings.end());
    if (!it->value.initialized) {
        global_object.vm().throw_exception<ReferenceError>(global_object, ErrorType::BindingNotInitialized, name);
        return {};
    }
    return it->value.value;
}

bool DeclarativeEnvironment::delete_binding(GlobalObject&, FlyString const& name)
{
    auto it = m_bindings.find(name);
    VERIFY(it != m_bindings.end());
    if (!it->value.can_be_deleted)
        return false;
    m_bindings.remove(it);
    return true;
}

}
