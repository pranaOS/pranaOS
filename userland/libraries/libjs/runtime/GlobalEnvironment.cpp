/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/AST.h>
#include <libjs/runtime/DeclarativeEnvironment.h>
#include <libjs/runtime/GlobalEnvironment.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/ObjectEnvironment.h>

namespace JS {

GlobalEnvironment::GlobalEnvironment(GlobalObject& global_object)
    : Environment(nullptr)
{
    m_object_record = global_object.heap().allocate<ObjectEnvironment>(global_object, global_object, ObjectEnvironment::IsWithEnvironment::No, nullptr);
    m_declarative_record = global_object.heap().allocate<DeclarativeEnvironment>(global_object);
}

void GlobalEnvironment::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(m_object_record);
    visitor.visit(m_declarative_record);
}

Optional<Variable> GlobalEnvironment::get_from_environment(FlyString const& name) const
{
    return m_object_record->get_from_environment(name);
}

bool GlobalEnvironment::put_into_environment(FlyString const& name, Variable variable)
{
    return m_object_record->put_into_environment(name, variable);
}

bool GlobalEnvironment::delete_from_environment(FlyString const& name)
{
    return object_record().delete_from_environment(name);
}

Value GlobalEnvironment::get_this_binding(GlobalObject&) const
{
    return &global_object();
}

Value GlobalEnvironment::global_this_value() const
{
    return &global_object();
}

bool GlobalEnvironment::has_binding(FlyString const& name) const
{
    if (m_declarative_record->has_binding(name))
        return true;
    return m_object_record->has_binding(name);
}

void GlobalEnvironment::create_mutable_binding(GlobalObject& global_object, FlyString const& name, bool can_be_deleted)
{
    if (m_declarative_record->has_binding(name)) {
        global_object.vm().throw_exception<TypeError>(global_object, ErrorType::FixmeAddAnErrorString);
        return;
    }
    m_declarative_record->create_mutable_binding(global_object, name, can_be_deleted);
}

void GlobalEnvironment::create_immutable_binding(GlobalObject& global_object, FlyString const& name, bool strict)
{
    if (m_declarative_record->has_binding(name)) {
        global_object.vm().throw_exception<TypeError>(global_object, ErrorType::FixmeAddAnErrorString);
        return;
    }
    m_declarative_record->create_immutable_binding(global_object, name, strict);
}

void GlobalEnvironment::initialize_binding(GlobalObject& global_object, FlyString const& name, Value value)
{
    if (m_declarative_record->has_binding(name)) {
        m_declarative_record->initialize_binding(global_object, name, value);
        return;
    }
    m_object_record->initialize_binding(global_object, name, value);
}

void GlobalEnvironment::set_mutable_binding(GlobalObject& global_object, FlyString const& name, Value value, bool strict)
{
    if (m_declarative_record->has_binding(name)) {
        m_declarative_record->set_mutable_binding(global_object, name, value, strict);
        return;
    }

    m_object_record->set_mutable_binding(global_object, name, value, strict);
}

Value GlobalEnvironment::get_binding_value(GlobalObject& global_object, FlyString const& name, bool strict)
{
    if (m_declarative_record->has_binding(name))
        return m_declarative_record->get_binding_value(global_object, name, strict);
    return m_object_record->get_binding_value(global_object, name, strict);
}

bool GlobalEnvironment::delete_binding(GlobalObject& global_object, FlyString const& name)
{
    if (m_declarative_record->has_binding(name))
        return m_declarative_record->delete_binding(global_object, name);

    bool existing_prop = m_object_record->binding_object().has_own_property(name);
    if (existing_prop) {
        bool status = m_object_record->delete_binding(global_object, name);
        if (status) {
            m_var_names.remove_all_matching([&](auto& entry) { return entry == name; });
        }
        return status;
    }
    return true;
}

bool GlobalEnvironment::has_var_declaration(FlyString const& name) const
{
    return m_var_names.contains_slow(name);
}

bool GlobalEnvironment::has_lexical_declaration(FlyString const& name) const
{
    return m_declarative_record->has_binding(name);
}

bool GlobalEnvironment::has_restricted_global_property(FlyString const& name) const
{
    auto& vm = this->vm();
    auto& global_object = m_object_record->binding_object();
    auto existing_prop = global_object.internal_get_own_property(name);
    if (vm.exception())
        return {};
    if (!existing_prop.has_value())
        return false;
    if (*existing_prop->configurable)
        return false;
    return true;
}

bool GlobalEnvironment::can_declare_global_var(FlyString const& name) const
{
    auto& vm = this->vm();
    auto& global_object = m_object_record->binding_object();
    bool has_property = global_object.has_own_property(name);
    if (vm.exception())
        return {};
    if (has_property)
        return true;
    return global_object.is_extensible();
}

bool GlobalEnvironment::can_declare_global_function(FlyString const& name) const
{
    auto& vm = this->vm();
    auto& global_object = m_object_record->binding_object();
    auto existing_prop = global_object.internal_get_own_property(name);
    if (vm.exception())
        return {};
    if (!existing_prop.has_value())
        return global_object.is_extensible();
    if (*existing_prop->configurable)
        return true;
    if (existing_prop->is_data_descriptor() && *existing_prop->writable && *existing_prop->enumerable)
        return true;
    return false;
}

void GlobalEnvironment::create_global_var_binding(FlyString const& name, bool can_be_deleted)
{
    auto& vm = this->vm();
    auto& global_object = m_object_record->binding_object();
    bool has_property = global_object.has_own_property(name);
    if (vm.exception())
        return;
    bool extensible = global_object.is_extensible();
    if (vm.exception())
        return;
    if (!has_property && extensible) {
        m_object_record->create_mutable_binding(m_object_record->global_object(), name, can_be_deleted);
        if (vm.exception())
            return;
        m_object_record->initialize_binding(m_object_record->global_object(), name, js_undefined());
        if (vm.exception())
            return;
    }
    if (!m_var_names.contains_slow(name))
        m_var_names.append(name);
}

void GlobalEnvironment::create_global_function_binding(FlyString const& name, Value value, bool can_be_deleted)
{
    auto& vm = this->vm();
    auto& global_object = m_object_record->binding_object();
    auto existing_prop = global_object.internal_get_own_property(name);
    if (vm.exception())
        return;
    PropertyDescriptor desc;
    if (!existing_prop.has_value() || *existing_prop->configurable)
        desc = { .value = value, .writable = true, .enumerable = true, .configurable = can_be_deleted };
    else
        desc = { .value = value };
    global_object.define_property_or_throw(name, desc);
    if (vm.exception())
        return;
    global_object.set(name, value, Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return;
    if (!m_var_names.contains_slow(name))
        m_var_names.append(name);
}

}
