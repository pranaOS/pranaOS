/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Function.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

Array* Array::create(GlobalObject& global_object, size_t length, Object* prototype)
{
    auto& vm = global_object.vm();
    if (length > NumericLimits<u32>::max()) {
        vm.throw_exception<RangeError>(global_object, ErrorType::InvalidLength, "array");
        return nullptr;
    }
    if (!prototype)
        prototype = global_object.array_prototype();
    auto* array = global_object.heap().allocate<Array>(global_object, *prototype);
    array->internal_define_own_property(vm.names.length, { .value = Value(length), .writable = true, .enumerable = false, .configurable = false });
    return array;
}

Array* Array::create_from(GlobalObject& global_object, Vector<Value> const& elements)
{
    auto* array = Array::create(global_object, 0);

    for (u32 n = 0; n < elements.size(); ++n) {

        array->create_data_property_or_throw(n, elements[n]);

    }

    return array;
}

Array::Array(Object& prototype)
    : Object(prototype)
{
}

Array::~Array()
{
}

bool Array::set_length(PropertyDescriptor const& property_descriptor)
{
    auto& global_object = this->global_object();
    auto& vm = this->vm();

    size_t new_length = indexed_properties().array_like_size();
    if (property_descriptor.value.has_value()) {

        new_length = property_descriptor.value->to_u32(global_object);
        if (vm.exception())
            return {};

        auto number_length = property_descriptor.value->to_number(global_object);
        if (vm.exception())
            return {};

        if (new_length != number_length.as_double()) {
            vm.throw_exception<RangeError>(global_object, ErrorType::InvalidLength, "array");
            return {};
        }
    }

    auto new_writable = property_descriptor.writable.value_or(true);

    if (property_descriptor.configurable.has_value() && *property_descriptor.configurable)
        return false;

    if (property_descriptor.enumerable.has_value() && *property_descriptor.enumerable)
        return false;

    if (property_descriptor.is_accessor_descriptor()) {

        return false;
    }

    if (!m_length_writable) {

        if (property_descriptor.writable.has_value() && *property_descriptor.writable)
            return false;

        if (new_length != indexed_properties().array_like_size())
            return false;
    }

    bool success = indexed_properties().set_array_like_size(new_length);

    if (!new_writable)
        m_length_writable = false;

    if (!success)
        return false;

    return true;
}

Optional<PropertyDescriptor> Array::internal_get_own_property(PropertyName const& property_name) const
{
    auto& vm = this->vm();
    if (property_name.is_string() && property_name.as_string() == vm.names.length.as_string())
        return PropertyDescriptor { .value = Value(indexed_properties().array_like_size()), .writable = m_length_writable, .enumerable = false, .configurable = false };

    return Object::internal_get_own_property(property_name);
}

bool Array::internal_define_own_property(PropertyName const& property_name, PropertyDescriptor const& property_descriptor)
{
    auto& vm = this->vm();

    VERIFY(property_name.is_valid());

    if (property_name.is_string() && property_name.as_string() == vm.names.length.as_string()) {

        return set_length(property_descriptor);
    }

    if (property_name.is_number()) {
        if (property_name.as_number() >= indexed_properties().array_like_size() && !m_length_writable)
            return false;

        auto succeeded = Object::internal_define_own_property(property_name, property_descriptor);
        if (!succeeded)
            return false;

        return true;
    }

    return Object::internal_define_own_property(property_name, property_descriptor);
}

bool Array::internal_delete(PropertyName const& property_name)
{
    auto& vm = this->vm();
    if (property_name.is_string() && property_name.as_string() == vm.names.length.as_string())
        return false;
    return Object::internal_delete(property_name);
}

MarkedValueList Array::internal_own_property_keys() const
{
    auto& vm = this->vm();
    auto keys = Object::internal_own_property_keys();
    if (vm.exception())
        return MarkedValueList { vm.heap() };

    keys.insert(indexed_properties().real_size(), js_string(vm, vm.names.length.as_string()));
    return keys;
}

}
