/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/AST.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Reference.h>

namespace JS {

void Reference::put_value(GlobalObject& global_object, Value value)
{
    auto& vm = global_object.vm();

    if (is_unresolvable()) {
        if (m_strict) {
            throw_reference_error(global_object);
            return;
        }
        global_object.set(m_name, value, Object::ShouldThrowExceptions::No);
        return;
    }

    if (is_property_reference()) {
        if (!m_base_value.is_object() && vm.in_strict_mode()) {
            if (m_base_value.is_nullish())
                vm.throw_exception<TypeError>(global_object, ErrorType::ReferenceNullishSetProperty, m_name, m_base_value.to_string_without_side_effects());
            else
                vm.throw_exception<TypeError>(global_object, ErrorType::ReferencePrimitiveSetProperty, m_name, m_base_value.typeof(), m_base_value.to_string_without_side_effects());
            return;
        }

        auto* base_obj = m_base_value.to_object(global_object);
        if (!base_obj)
            return;

        bool succeeded = base_obj->internal_set(m_name, value, get_this_value());
        if (vm.exception())
            return;
        if (!succeeded && m_strict) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ReferenceNullishSetProperty, m_name, m_base_value.to_string_without_side_effects());
            return;
        }
        return;
    }

    VERIFY(m_base_type == BaseType::Environment);
    auto existing_variable = m_base_environment->get_from_environment(m_name.as_string());
    Variable variable {
        .value = value,
        .declaration_kind = existing_variable.has_value() ? existing_variable->declaration_kind : DeclarationKind::Var
    };

    if (variable.declaration_kind == DeclarationKind::Const) {
        vm.throw_exception<TypeError>(global_object, ErrorType::InvalidAssignToConst);
        return;
    }

    bool succeeded = m_base_environment->put_into_environment(m_name.as_string(), variable);
    if (vm.exception())
        return;

    if (!succeeded && m_strict) {
        vm.throw_exception<TypeError>(global_object, ErrorType::DescWriteNonWritable, m_name);
        return;
    }
}

void Reference::throw_reference_error(GlobalObject& global_object)
{
    auto& vm = global_object.vm();
    if (!m_name.is_valid())
        vm.throw_exception<ReferenceError>(global_object, ErrorType::ReferenceUnresolvable);
    else
        vm.throw_exception<ReferenceError>(global_object, ErrorType::UnknownIdentifier, m_name.to_string_or_symbol().to_display_string());
}

Value Reference::get_value(GlobalObject& global_object, bool throw_if_undefined)
{
    if (is_unresolvable()) {
        throw_reference_error(global_object);
        return {};
    }

    if (is_property_reference()) {
        auto* base_obj = m_base_value.to_object(global_object);
        if (!base_obj)
            return {};
        return base_obj->get(m_name);
    }

    VERIFY(m_base_type == BaseType::Environment);
    auto value = m_base_environment->get_from_environment(m_name.as_string());
    if (!value.has_value()) {
        if (!throw_if_undefined) {
            return js_undefined();
        }
        throw_reference_error(global_object);
        return {};
    }
    return value->value;
}

bool Reference::delete_(GlobalObject& global_object)
{
    if (is_unresolvable()) {
        VERIFY(!m_strict);
    
        return true;
    }

    auto& vm = global_object.vm();

    if (is_property_reference()) {
        
        if (is_super_reference()) {
            vm.throw_exception<ReferenceError>(global_object, ErrorType::UnsupportedDeleteSuperProperty);
            return {};
        }

        auto* base_obj = m_base_value.to_object(global_object);
        VERIFY(base_obj);

        bool delete_status = base_obj->internal_delete(m_name);
        if (vm.exception())
            return {};

        if (!delete_status && m_strict) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ReferenceNullishDeleteProperty, m_name, m_base_value.to_string_without_side_effects());
            return {};
        }

        return delete_status;
    }

    VERIFY(m_base_type == BaseType::Environment);

    return m_base_environment->delete_from_environment(m_name.as_string());
}

String Reference::to_string() const
{
    StringBuilder builder;
    builder.append("Reference { Base=");
    switch (m_base_type) {
    case BaseType::Unresolvable:
        builder.append("Unresolvable");
        break;
    case BaseType::Environment:
        builder.appendff("{}", base_environment().class_name());
        break;
    case BaseType::Value:
        if (m_base_value.is_empty())
            builder.append("<empty>");
        else
            builder.appendff("{}", m_base_value.to_string_without_side_effects());
        break;
    }
    builder.append(", ReferencedName=");
    if (!m_name.is_valid())
        builder.append("<invalid>");
    else if (m_name.is_symbol())
        builder.appendff("{}", m_name.as_symbol()->to_string());
    else
        builder.appendff("{}", m_name.to_string());
    builder.appendff(", Strict={}", m_strict);
    builder.appendff(", ThisValue=");
    if (m_this_value.is_empty())
        builder.append("<empty>");
    else
        builder.appendff("{}", m_this_value.to_string_without_side_effects());

    builder.append(" }");
    return builder.to_string();
}

}
