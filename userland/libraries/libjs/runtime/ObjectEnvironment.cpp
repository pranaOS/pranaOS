/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/AST.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/ObjectEnvironment.h>

namespace JS {

ObjectEnvironment::ObjectEnvironment(Object& binding_object, IsWithEnvironment is_with_environment, Environment* outer_environment)
    : Environment(outer_environment)
    , m_binding_object(binding_object)
    , m_with_environment(is_with_environment == IsWithEnvironment::Yes)
{
}

void ObjectEnvironment::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(&m_binding_object);
}

Optional<Variable> ObjectEnvironment::get_from_environment(FlyString const& name) const
{
    if (!m_binding_object.storage_has(name))
        return {};
    auto value = m_binding_object.get(name);
    return Variable { value, DeclarationKind::Var };
}

bool ObjectEnvironment::put_into_environment(FlyString const& name, Variable variable)
{
    return m_binding_object.set(name, variable.value, Object::ShouldThrowExceptions::No);
}

bool ObjectEnvironment::delete_from_environment(FlyString const& name)
{
    return m_binding_object.internal_delete(name);
}

bool ObjectEnvironment::has_binding(FlyString const& name) const
{
    auto& vm = this->vm();
    bool found_binding = m_binding_object.has_property(name);
    if (vm.exception())
        return {};
    if (!found_binding)
        return false;
    if (!m_with_environment)
        return true;
    auto unscopables = m_binding_object.get(*vm.well_known_symbol_unscopables());
    if (vm.exception())
        return {};
    if (unscopables.is_object()) {
        auto blocked = unscopables.as_object().get(name);
        if (vm.exception())
            return {};
        if (blocked.to_boolean())
            return false;
    }
    return true;
}

void ObjectEnvironment::create_mutable_binding(GlobalObject&, FlyString const& name, bool can_be_deleted)
{
    m_binding_object.define_property_or_throw(name, { .value = js_undefined(), .writable = true, .enumerable = true, .configurable = can_be_deleted });
}

void ObjectEnvironment::create_immutable_binding(GlobalObject&, FlyString const&, bool)
{
    VERIFY_NOT_REACHED();
}

void ObjectEnvironment::initialize_binding(GlobalObject& global_object, FlyString const& name, Value value)
{
    set_mutable_binding(global_object, name, value, false);
}

void ObjectEnvironment::set_mutable_binding(GlobalObject& global_object, FlyString const& name, Value value, bool strict)
{
    auto& vm = this->vm();
    bool still_exists = m_binding_object.has_property(name);
    if (vm.exception())
        return;
    if (!still_exists && strict) {
        global_object.vm().throw_exception<ReferenceError>(global_object, ErrorType::UnknownIdentifier, name);
        return;
    }
    m_binding_object.set(name, value, strict ? Object::ShouldThrowExceptions::Yes : Object::ShouldThrowExceptions::No);
}

Value ObjectEnvironment::get_binding_value(GlobalObject& global_object, FlyString const& name, bool strict)
{
    auto& vm = this->vm();
    auto value = m_binding_object.has_property(name);
    if (vm.exception())
        return {};
    if (!value) {
        if (!strict)
            return js_undefined();

        global_object.vm().throw_exception<ReferenceError>(global_object, ErrorType::UnknownIdentifier, name);
        return {};
    }
    return m_binding_object.get(name);
}

bool ObjectEnvironment::delete_binding(GlobalObject&, FlyString const& name)
{
    return m_binding_object.internal_delete(name);
}

}
