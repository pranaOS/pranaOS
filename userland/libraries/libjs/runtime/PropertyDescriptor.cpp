/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/PropertyDescriptor.h>
#include <libjs/runtime/Value.h>

namespace JS {

bool PropertyDescriptor::is_accessor_descriptor() const
{

    if (!get.has_value() && !set.has_value())
        return false;

    return true;
}

bool PropertyDescriptor::is_data_descriptor() const
{
    if (!value.has_value() && !writable.has_value())
        return false;

    return true;
}

bool PropertyDescriptor::is_generic_descriptor() const
{
    if (!is_accessor_descriptor() && !is_data_descriptor())
        return true;

    return false;
}

Value from_property_descriptor(GlobalObject& global_object, Optional<PropertyDescriptor> const& property_descriptor)
{
    if (!property_descriptor.has_value())
        return js_undefined();
    auto& vm = global_object.vm();
    auto* object = Object::create(global_object, global_object.object_prototype());
    if (property_descriptor->value.has_value())
        object->create_data_property_or_throw(vm.names.value, *property_descriptor->value);
    if (property_descriptor->writable.has_value())
        object->create_data_property_or_throw(vm.names.writable, Value(*property_descriptor->writable));
    if (property_descriptor->get.has_value())
        object->create_data_property_or_throw(vm.names.get, *property_descriptor->get ? Value(*property_descriptor->get) : js_undefined());
    if (property_descriptor->set.has_value())
        object->create_data_property_or_throw(vm.names.set, *property_descriptor->set ? Value(*property_descriptor->set) : js_undefined());
    if (property_descriptor->enumerable.has_value())
        object->create_data_property_or_throw(vm.names.enumerable, Value(*property_descriptor->enumerable));
    if (property_descriptor->configurable.has_value())
        object->create_data_property_or_throw(vm.names.configurable, Value(*property_descriptor->configurable));
    return object;
}

PropertyDescriptor to_property_descriptor(GlobalObject& global_object, Value argument)
{
    auto& vm = global_object.vm();
    if (!argument.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, argument.to_string_without_side_effects());
        return {};
    }
    auto& object = argument.as_object();
    PropertyDescriptor descriptor;
    auto has_enumerable = object.has_property(vm.names.enumerable);
    if (vm.exception())
        return {};
    if (has_enumerable) {
        auto enumerable = object.get(vm.names.enumerable);
        if (vm.exception())
            return {};
        descriptor.enumerable = enumerable.to_boolean();
    }
    auto has_configurable = object.has_property(vm.names.configurable);
    if (vm.exception())
        return {};
    if (has_configurable) {
        auto configurable = object.get(vm.names.configurable);
        if (vm.exception())
            return {};
        descriptor.configurable = configurable.to_boolean();
    }
    auto has_value = object.has_property(vm.names.value);
    if (vm.exception())
        return {};
    if (has_value) {
        auto value = object.get(vm.names.value);
        if (vm.exception())
            return {};
        descriptor.value = value;
    }
    auto has_writable = object.has_property(vm.names.writable);
    if (vm.exception())
        return {};
    if (has_writable) {
        auto writable = object.get(vm.names.writable);
        if (vm.exception())
            return {};
        descriptor.writable = writable.to_boolean();
    }
    auto has_get = object.has_property(vm.names.get);
    if (vm.exception())
        return {};
    if (has_get) {
        auto getter = object.get(vm.names.get);
        if (vm.exception())
            return {};
        if (!getter.is_function() && !getter.is_undefined()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::AccessorBadField, "get");
            return {};
        }
        descriptor.get = getter.is_function() ? &getter.as_function() : nullptr;
    }
    auto has_set = object.has_property(vm.names.set);
    if (vm.exception())
        return {};
    if (has_set) {
        auto setter = object.get(vm.names.set);
        if (vm.exception())
            return {};
        if (!setter.is_function() && !setter.is_undefined()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::AccessorBadField, "set");
            return {};
        }
        descriptor.set = setter.is_function() ? &setter.as_function() : nullptr;
    }
    if (descriptor.get.has_value() || descriptor.set.has_value()) {
        if (descriptor.value.has_value() || descriptor.writable.has_value()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::AccessorValueOrWritable);
            return {};
        }
    }
    return descriptor;
}


void PropertyDescriptor::complete()
{
    if (is_generic_descriptor() || is_data_descriptor()) {
        if (!value.has_value())
            value = Value {};
        if (!writable.has_value())
            writable = false;
    } else {
        if (!get.has_value())
            get = nullptr;
        if (!set.has_value())
            set = nullptr;
    }
    if (!enumerable.has_value())
        enumerable = false;
    if (!configurable.has_value())
        configurable = false;
}

PropertyAttributes PropertyDescriptor::attributes() const
{
    u8 attributes = 0;
    if (writable.value_or(false))
        attributes |= Attribute::Writable;
    if (enumerable.value_or(false))
        attributes |= Attribute::Enumerable;
    if (configurable.value_or(false))
        attributes |= Attribute::Configurable;
    return { attributes };
}

}
