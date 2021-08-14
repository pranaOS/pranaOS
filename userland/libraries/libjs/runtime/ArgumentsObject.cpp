/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/ArgumentsObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

ArgumentsObject::ArgumentsObject(GlobalObject& global_object, Environment& environment)
    : Object(*global_object.object_prototype())
    , m_environment(environment)
{
}

void ArgumentsObject::initialize(GlobalObject& global_object)
{
    Base::initialize(global_object);
    set_has_parameter_map();
    m_parameter_map = Object::create(global_object, nullptr);
}

ArgumentsObject::~ArgumentsObject()
{
}

void ArgumentsObject::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(&m_environment);
    visitor.visit(m_parameter_map);
}

Value ArgumentsObject::internal_get(PropertyName const& property_name, Value receiver) const
{
    auto& map = *m_parameter_map;

    bool is_mapped = m_parameter_map->has_own_property(property_name);
    if (!is_mapped) {

        return Object::internal_get(property_name, receiver);
    }

    return map.get(property_name);
}

bool ArgumentsObject::internal_set(PropertyName const& property_name, Value value, Value receiver)
{
    bool is_mapped = false;

    if (!same_value(this, receiver)) {

        is_mapped = false;
    } else {

        is_mapped = parameter_map().has_own_property(property_name);
    }

    if (is_mapped) {

        auto set_status = m_parameter_map->set(property_name, value, Object::ShouldThrowExceptions::No);
        VERIFY(set_status);
    }

    return Object::internal_set(property_name, value, receiver);
}

bool ArgumentsObject::internal_delete(PropertyName const& property_name)
{
    auto& map = parameter_map();
    bool is_mapped = map.has_own_property(property_name);
    bool result = Object::internal_delete(property_name);
    if (vm().exception())
        return false;

    if (result && is_mapped) {

        map.internal_delete(property_name);
    }

    return result;
}

Optional<PropertyDescriptor> ArgumentsObject::internal_get_own_property(PropertyName const& property_name) const
{
    auto desc = Object::internal_get_own_property(property_name);    
    if (!desc.has_value())
        return desc;

    bool is_mapped = m_parameter_map->has_own_property(property_name);

    if (is_mapped) {

        desc->value = m_parameter_map->get(property_name);
    }
    return desc;
}

bool ArgumentsObject::internal_define_own_property(PropertyName const& property_name, PropertyDescriptor const& descriptor)
{
    auto& map = parameter_map();

    bool is_mapped = map.has_own_property(property_name);

    auto new_arg_desc = descriptor;

    if (is_mapped && descriptor.is_data_descriptor()) {

        if (!descriptor.value.has_value() && descriptor.writable.has_value() && descriptor.writable == false) {

            new_arg_desc = descriptor;
            new_arg_desc.value = map.get(property_name);
        }
    }

    bool allowed = Object::internal_define_own_property(property_name, new_arg_desc);
    if (vm().exception())
        return false;

    if (!allowed)
        return false;


    if (is_mapped) {

        if (descriptor.is_accessor_descriptor()) {

            map.internal_delete(property_name);
        } else {

            if (descriptor.value.has_value()) {
                
                bool set_status = map.set(property_name, descriptor.value.value(), Object::ShouldThrowExceptions::No);

                VERIFY(set_status);
            }

            if (descriptor.writable == false) {
                map.internal_delete(property_name);
            }
        }
    }

    return true;
}

}
