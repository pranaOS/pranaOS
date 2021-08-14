/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Accessor.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/PropertyDescriptor.h>
#include <libjs/runtime/ProxyObject.h>

namespace JS {

ProxyObject* ProxyObject::create(GlobalObject& global_object, Object& target, Object& handler)
{
    return global_object.heap().allocate<ProxyObject>(global_object, target, handler, *global_object.object_prototype());
}

ProxyObject::ProxyObject(Object& target, Object& handler, Object& prototype)
    : FunctionObject(prototype)
    , m_target(target)
    , m_handler(handler)
{
}

ProxyObject::~ProxyObject()
{
}

static Value property_name_to_value(VM& vm, PropertyName const& name)
{
    VERIFY(name.is_valid());
    if (name.is_symbol())
        return name.as_symbol();

    if (name.is_string())
        return js_string(vm, name.as_string());

    VERIFY(name.is_number());
    return js_string(vm, String::number(name.as_number()));
}

Object* ProxyObject::internal_get_prototype_of() const
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


   
    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.getPrototypeOf);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_get_prototype_of();
    }

    auto handler_proto = vm.call(*trap, &m_handler, &m_target);
    if (vm.exception())
        return {};

    if (!handler_proto.is_object() && !handler_proto.is_null()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetPrototypeOfReturn);
        return {};
    }

    auto extensible_target = m_target.is_extensible();
    if (vm.exception())
        return {};

    if (extensible_target)
        return handler_proto.is_null() ? nullptr : &handler_proto.as_object();

    auto target_proto = m_target.internal_get_prototype_of();
    if (vm.exception())
        return {};

    if (!same_value(handler_proto, target_proto)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetPrototypeOfNonExtensible);
        return {};
    }

    return handler_proto.is_null() ? nullptr : &handler_proto.as_object();
}

bool ProxyObject::internal_set_prototype_of(Object* prototype)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.setPrototypeOf);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_set_prototype_of(prototype);
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target, prototype);
    if (vm.exception())
        return {};

    if (!trap_result.to_boolean())
        return false;

    auto extensible_target = m_target.is_extensible();
    if (vm.exception())
        return {};

    if (extensible_target)
        return true;

    auto* target_proto = m_target.internal_get_prototype_of();
    if (vm.exception())
        return {};

    if (!same_value(prototype, target_proto)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxySetPrototypeOfNonExtensible);
        return {};
    }

    return true;
}

bool ProxyObject::internal_is_extensible() const
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.isExtensible);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.is_extensible();
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target);
    if (vm.exception())
        return {};

    auto target_result = m_target.is_extensible();
    if (vm.exception())
        return {};

    if (trap_result.to_boolean() != target_result) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyIsExtensibleReturn);
        return {};
    }

    return trap_result.to_boolean();
}

bool ProxyObject::internal_prevent_extensions()
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.preventExtensions);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_prevent_extensions();
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target);
    if (vm.exception())
        return {};

    if (trap_result.to_boolean()) {
        auto extensible_target = m_target.is_extensible();
        if (vm.exception())
            return {};

        if (extensible_target) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyPreventExtensionsReturn);
            return {};
        }
    }

    return trap_result.to_boolean();
}

Optional<PropertyDescriptor> ProxyObject::internal_get_own_property(const PropertyName& property_name) const
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    VERIFY(property_name.is_valid());


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.getOwnPropertyDescriptor);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_get_own_property(property_name);
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target, property_name_to_value(vm, property_name));
    if (vm.exception())
        return {};

    if (!trap_result.is_object() && !trap_result.is_undefined()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetOwnDescriptorReturn);
        return {};
    }

    auto target_descriptor = m_target.internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (trap_result.is_undefined()) {
        if (!target_descriptor.has_value())
            return {};

        if (!*target_descriptor->configurable) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetOwnDescriptorNonConfigurable);
            return {};
        }

        auto extensible_target = m_target.is_extensible();
        if (vm.exception())
            return {};

        if (!extensible_target) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetOwnDescriptorUndefinedReturn);
            return {};
        }

        return {};
    }

    auto extensible_target = m_target.is_extensible();
    if (vm.exception())
        return {};

    auto result_desc = to_property_descriptor(global_object, trap_result);
    if (vm.exception())
        return {};

    result_desc.complete();

    auto valid = is_compatible_property_descriptor(extensible_target, result_desc, target_descriptor);

    if (!valid) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetOwnDescriptorInvalidDescriptor);
        return {};
    }

    if (!*result_desc.configurable) {
        if (!target_descriptor.has_value() || *target_descriptor->configurable) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetOwnDescriptorInvalidNonConfig);
            return {};
        }
        if (result_desc.writable.has_value() && !*result_desc.writable) {
            if (*target_descriptor->writable) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetOwnDescriptorNonConfigurableNonWritable);
                return {};
            }
        }
    }

    return result_desc;
}

bool ProxyObject::internal_define_own_property(PropertyName const& property_name, PropertyDescriptor const& property_descriptor)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    VERIFY(property_name.is_valid());


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.defineProperty);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_define_own_property(property_name, property_descriptor);
    }

    auto descriptor_object = from_property_descriptor(global_object, property_descriptor);

    auto trap_result = vm.call(*trap, &m_handler, &m_target, property_name_to_value(vm, property_name), descriptor_object);
    if (vm.exception())
        return {};

    if (!trap_result.to_boolean())
        return false;

    auto target_descriptor = m_target.internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    auto extensible_target = m_target.is_extensible();
    if (vm.exception())
        return {};

    bool setting_config_false = false;

    if (property_descriptor.configurable.has_value() && !*property_descriptor.configurable) {
        setting_config_false = true;
    }

    if (!target_descriptor.has_value()) {
        if (!extensible_target) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDefinePropNonExtensible);
            return {};
        }
        if (setting_config_false) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDefinePropNonConfigurableNonExisting);
            return {};
        }
    }
    else {
        if (!is_compatible_property_descriptor(extensible_target, property_descriptor, target_descriptor)) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDefinePropIncompatibleDescriptor);
            return {};
        }
        if (setting_config_false && *target_descriptor->configurable) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDefinePropExistingConfigurable);
            return {};
        }
        if (target_descriptor->is_data_descriptor() && !*target_descriptor->configurable && *target_descriptor->writable) {
            if (property_descriptor.writable.has_value() && !*property_descriptor.writable) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDefinePropNonWritable);
                return {};
            }
        }
    }

    return true;
}

bool ProxyObject::internal_has_property(PropertyName const& property_name) const
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    VERIFY(property_name.is_valid());


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.has);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_has_property(property_name);
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target, property_name_to_value(vm, property_name));
    if (vm.exception())
        return {};

    if (!trap_result.to_boolean()) {
        auto target_descriptor = m_target.internal_get_own_property(property_name);
        if (vm.exception())
            return {};

        if (target_descriptor.has_value()) {
            if (!*target_descriptor->configurable) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxyHasExistingNonConfigurable);
                return {};
            }

            auto extensible_target = m_target.is_extensible();
            if (vm.exception())
                return {};

            if (!extensible_target) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxyHasExistingNonExtensible);
                return false;
            }
        }
    }

    return trap_result.to_boolean();
}

Value ProxyObject::internal_get(PropertyName const& property_name, Value receiver) const
{
    VERIFY(!receiver.is_empty());

    auto& vm = this->vm();
    auto& global_object = this->global_object();

    VERIFY(property_name.is_valid());


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.get);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_get(property_name, receiver);
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target, property_name_to_value(vm, property_name), receiver);
    if (vm.exception())
        return {};

    auto target_descriptor = m_target.internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (target_descriptor.has_value() && !*target_descriptor->configurable) {
        if (target_descriptor->is_data_descriptor() && !*target_descriptor->writable) {
            if (!same_value(trap_result, *target_descriptor->value)) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetImmutableDataProperty);
                return {};
            }
        }
        if (target_descriptor->is_accessor_descriptor() && !*target_descriptor->get) {
            if (!trap_result.is_undefined()) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxyGetNonConfigurableAccessor);
                return {};
            }
        }
    }

    return trap_result;
}

bool ProxyObject::internal_set(PropertyName const& property_name, Value value, Value receiver)
{
    VERIFY(!value.is_empty());
    VERIFY(!receiver.is_empty());

    auto& vm = this->vm();
    auto& global_object = this->global_object();

    VERIFY(property_name.is_valid());


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.set);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_set(property_name, value, receiver);
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target, property_name_to_value(vm, property_name), value, receiver);
    if (vm.exception())
        return {};

    if (!trap_result.to_boolean())
        return false;

    auto target_descriptor = m_target.internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (target_descriptor.has_value() && !*target_descriptor->configurable) {
        if (target_descriptor->is_data_descriptor() && !*target_descriptor->writable) {
            if (!same_value(value, *target_descriptor->value)) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxySetImmutableDataProperty);
                return {};
            }
        }
        if (target_descriptor->is_accessor_descriptor()) {
            if (!*target_descriptor->set) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ProxySetNonConfigurableAccessor);
                return {};
            }
        }
    }

    return true;
}

bool ProxyObject::internal_delete(PropertyName const& property_name)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    VERIFY(property_name.is_valid());


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.deleteProperty);
    if (vm.exception())
        return {};

    if (!trap) {
        return m_target.internal_delete(property_name);
    }

    auto trap_result = vm.call(*trap, &m_handler, &m_target, property_name_to_value(vm, property_name));
    if (vm.exception())
        return {};

    if (!trap_result.to_boolean())
        return false;

    auto target_descriptor = m_target.internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (!target_descriptor.has_value())
        return true;

    if (!*target_descriptor->configurable) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDeleteNonConfigurable);
        return {};
    }

    auto extensible_target = m_target.is_extensible();
    if (vm.exception())
        return {};

    if (!extensible_target) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyDeleteNonExtensible);
        return {};
    }

    return true;
}

MarkedValueList ProxyObject::internal_own_property_keys() const
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return MarkedValueList { heap() };
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.ownKeys);
    if (vm.exception())
        return MarkedValueList { heap() };

    if (!trap) {
        return m_target.internal_own_property_keys();
    }

    auto trap_result_array = vm.call(*trap, &m_handler, &m_target);
    if (vm.exception())
        return MarkedValueList { heap() };

    HashTable<StringOrSymbol> unique_keys;
    auto trap_result = create_list_from_array_like(global_object, trap_result_array, [&](auto value) {
        auto& vm = global_object.vm();
        if (!value.is_string() && !value.is_symbol()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ProxyOwnPropertyKeysNotStringOrSymbol);
            return;
        }
        auto property_key = value.to_property_key(global_object);
        VERIFY(!vm.exception());
        unique_keys.set(property_key, Base::HashSetExistingEntryBehavior::Keep);
    });

    if (unique_keys.size() != trap_result.size()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyOwnPropertyKeysDuplicates);
        return MarkedValueList { heap() };
    }

    auto extensible_target = m_target.is_extensible();
    if (vm.exception())
        return MarkedValueList { heap() };

    auto target_keys = m_target.internal_own_property_keys();
    if (vm.exception())
        return MarkedValueList { heap() };


    auto target_configurable_keys = MarkedValueList { heap() };

    auto target_nonconfigurable_keys = MarkedValueList { heap() };

    for (auto& key : target_keys) {
        auto descriptor = m_target.internal_get_own_property(PropertyName::from_value(global_object, key));

        if (descriptor.has_value() && !*descriptor->configurable) {
            target_nonconfigurable_keys.append(key);
        }
        else {
            target_configurable_keys.append(key);
        }
    }

    if (extensible_target && target_nonconfigurable_keys.is_empty()) {
        return trap_result;
    }

    auto unchecked_result_keys = MarkedValueList { heap() };
    unchecked_result_keys.extend(trap_result);

    for (auto& key : target_nonconfigurable_keys) {
        if (!unchecked_result_keys.contains_slow(key)) {
            vm.throw_exception<TypeError>(global_object, ErrorType::FixmeAddAnErrorString);
            return MarkedValueList { heap() };
        }

        unchecked_result_keys.remove_first_matching([&](auto& value) {
            return same_value(value, key);
        });
    }

    if (extensible_target)
        return trap_result;

    for (auto& key : target_configurable_keys) {
        if (!unchecked_result_keys.contains_slow(key)) {
            vm.throw_exception<TypeError>(global_object, ErrorType::FixmeAddAnErrorString);
            return MarkedValueList { heap() };
        }

        unchecked_result_keys.remove_first_matching([&](auto& value) {
            return same_value(value, key);
        });
    }

    if (!unchecked_result_keys.is_empty()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::FixmeAddAnErrorString);
        return MarkedValueList { heap() };
    }

    return trap_result;
}

Value ProxyObject::call()
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();
    auto this_argument = vm.this_value(global_object);

    if (!is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, Value(this).to_string_without_side_effects());
        return {};
    }


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.apply);
    if (vm.exception())
        return {};

    if (!trap) {
        return static_cast<FunctionObject&>(m_target).call();
    }

    auto arguments_array = Array::create(global_object, 0);
    vm.for_each_argument([&](auto& argument) {
        arguments_array->indexed_properties().append(argument);
    });

    return vm.call(*trap, &m_handler, &m_target, this_argument, arguments_array);
}

Value ProxyObject::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    if (!is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAConstructor, Value(this).to_string_without_side_effects());
        return {};
    }


    if (m_is_revoked) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyRevoked);
        return {};
    }


    auto trap = Value(&m_handler).get_method(global_object, vm.names.construct);
    if (vm.exception())
        return {};

    if (!trap) {
        return static_cast<FunctionObject&>(m_target).construct(new_target);
    }

    auto arguments_array = Array::create(global_object, 0);
    vm.for_each_argument([&](auto& argument) {
        arguments_array->indexed_properties().append(argument);
    });

    auto result = vm.call(*trap, &m_handler, &m_target, arguments_array, &new_target);

    if (!result.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyConstructBadReturnType);
        return {};
    }

    return result;
}

void ProxyObject::visit_edges(Cell::Visitor& visitor)
{
    FunctionObject::visit_edges(visitor);
    visitor.visit(&m_target);
    visitor.visit(&m_handler);
}

const FlyString& ProxyObject::name() const
{
    VERIFY(is_function());
    return static_cast<FunctionObject&>(m_target).name();
}

FunctionEnvironment* ProxyObject::create_environment(FunctionObject& function_being_invoked)
{
    VERIFY(is_function());
    return static_cast<FunctionObject&>(m_target).create_environment(function_being_invoked);
}

}
