/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Utf16View.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/PrimitiveString.h>
#include <libjs/runtime/PropertyDescriptor.h>
#include <libjs/runtime/StringObject.h>

namespace JS {

StringObject* StringObject::create(GlobalObject& global_object, PrimitiveString& primitive_string, Object& prototype)
{
    return global_object.heap().allocate<StringObject>(global_object, primitive_string, prototype);
}

StringObject::StringObject(PrimitiveString& string, Object& prototype)
    : Object(prototype)
    , m_string(string)
{
}

StringObject::~StringObject()
{
}

void StringObject::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    define_direct_property(vm.names.length, Value(m_string.utf16_string_view().length_in_code_units()), 0);
}

void StringObject::visit_edges(Cell::Visitor& visitor)
{
    Object::visit_edges(visitor);
    visitor.visit(&m_string);
}

static Optional<PropertyDescriptor> string_get_own_property(GlobalObject& global_object, StringObject const& string, PropertyName const& property_name)
{
    VERIFY(property_name.is_valid());

    if (property_name.is_symbol())
        return {};

    auto index = canonical_numeric_index_string(global_object, property_name);

    if (index.is_undefined())
        return {};

    if (!index.is_integral_number())
        return {};

    if (index.is_negative_zero())
        return {};

    auto str = string.primitive_string().utf16_string_view();

    auto length = str.length_in_code_units();

    if (index.as_double() < 0 || length <= index.as_double())
        return {};

    auto result_str = js_string(string.vm(), str.substring_view(index.as_double(), 1));

    return PropertyDescriptor {
        .value = result_str,
        .writable = false,
        .enumerable = true,
        .configurable = false,
    };
}

Optional<PropertyDescriptor> StringObject::internal_get_own_property(PropertyName const& property_name) const
{
    auto descriptor = Object::internal_get_own_property(property_name);

    if (descriptor.has_value())
        return descriptor;

    return string_get_own_property(global_object(), *this, property_name);
}

bool StringObject::internal_define_own_property(PropertyName const& property_name, PropertyDescriptor const& property_descriptor)
{
    VERIFY(property_name.is_valid());

    auto string_descriptor = string_get_own_property(global_object(), *this, property_name);

    if (string_descriptor.has_value()) {
        auto extensible = m_is_extensible;
        return is_compatible_property_descriptor(extensible, property_descriptor, string_descriptor);
    }

    return Object::internal_define_own_property(property_name, property_descriptor);
}

MarkedValueList StringObject::internal_own_property_keys() const
{
    auto& vm = this->vm();
    auto keys = MarkedValueList { heap() };

    auto str = m_string.utf16_string_view();

    auto length = str.length_in_code_units();

    for (size_t i = 0; i < length; ++i) {
        keys.append(js_string(vm, String::number(i)));
    }

    for (auto& entry : indexed_properties()) {
        if (entry.index() >= length) {
            keys.append(js_string(vm, String::number(entry.index())));
        }
    }

    for (auto& it : shape().property_table_ordered()) {
        if (it.key.is_string()) {
            keys.append(it.key.to_value(vm));
        }
    }

    for (auto& it : shape().property_table_ordered()) {
        if (it.key.is_symbol()) {
            keys.append(it.key.to_value(vm));
        }
    }

    return keys;
}

}
