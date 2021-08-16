/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Function.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/IteratorOperations.h>
#include <libjs/runtime/ObjectConstructor.h>
#include <libjs/runtime/ProxyObject.h>
#include <libjs/runtime/Shape.h>

namespace JS {

ObjectConstructor::ObjectConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Object.as_string(), *global_object.function_prototype())
{
}

void ObjectConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.object_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.defineProperty, define_property, 3, attr);
    define_native_function(vm.names.defineProperties, define_properties, 2, attr);
    define_native_function(vm.names.is, is, 2, attr);
    define_native_function(vm.names.getOwnPropertyDescriptor, get_own_property_descriptor, 2, attr);
    define_native_function(vm.names.getOwnPropertyDescriptors, get_own_property_descriptors, 1, attr);
    define_native_function(vm.names.getOwnPropertyNames, get_own_property_names, 1, attr);
    define_native_function(vm.names.getOwnPropertySymbols, get_own_property_symbols, 1, attr);
    define_native_function(vm.names.getPrototypeOf, get_prototype_of, 1, attr);
    define_native_function(vm.names.setPrototypeOf, set_prototype_of, 2, attr);
    define_native_function(vm.names.isExtensible, is_extensible, 1, attr);
    define_native_function(vm.names.isFrozen, is_frozen, 1, attr);
    define_native_function(vm.names.isSealed, is_sealed, 1, attr);
    define_native_function(vm.names.preventExtensions, prevent_extensions, 1, attr);
    define_native_function(vm.names.freeze, freeze, 1, attr);
    define_native_function(vm.names.fromEntries, from_entries, 1, attr);
    define_native_function(vm.names.seal, seal, 1, attr);
    define_native_function(vm.names.keys, keys, 1, attr);
    define_native_function(vm.names.values, values, 1, attr);
    define_native_function(vm.names.entries, entries, 1, attr);
    define_native_function(vm.names.create, create, 2, attr);
    define_native_function(vm.names.hasOwn, has_own, 2, attr);
    define_native_function(vm.names.assign, assign, 2, attr);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

ObjectConstructor::~ObjectConstructor()
{
}

Value ObjectConstructor::call()
{
    return construct(*this);
}

Value ObjectConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    if (&new_target != this)
        return ordinary_create_from_constructor<Object>(global_object, new_target, &GlobalObject::object_prototype);
    auto value = vm.argument(0);
    if (value.is_nullish())
        return Object::create(global_object, global_object.object_prototype());
    return value.to_object(global_object);
}

enum class GetOwnPropertyKeysType {
    String,
    Symbol,
};

static Array* get_own_property_keys(GlobalObject& global_object, Value value, GetOwnPropertyKeysType type)
{
    auto& vm = global_object.vm();

    auto* object = value.to_object(global_object);
    if (vm.exception())
        return {};

    auto keys = object->internal_own_property_keys();
    if (vm.exception())
        return {};

    auto name_list = MarkedValueList { vm.heap() };

    for (auto& next_key : keys) {
        if ((next_key.is_symbol() && type == GetOwnPropertyKeysType::Symbol) || (next_key.is_string() && type == GetOwnPropertyKeysType::String)) {
            name_list.append(next_key);
        }
    }

    return Array::create_from(global_object, name_list);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::get_own_property_names)
{
    return get_own_property_keys(global_object, vm.argument(0), GetOwnPropertyKeysType::String);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::get_own_property_symbols)
{
    return get_own_property_keys(global_object, vm.argument(0), GetOwnPropertyKeysType::Symbol);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::get_prototype_of)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};

    return object->internal_get_prototype_of();
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::set_prototype_of)
{
    auto proto = vm.argument(1);

    auto object = require_object_coercible(global_object, vm.argument(0));
    if (vm.exception())
        return {};

    if (!proto.is_object() && !proto.is_null()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectPrototypeWrongType);
        return {};
    }

    if (!object.is_object())
        return object;

    auto status = object.as_object().internal_set_prototype_of(proto.is_null() ? nullptr : &proto.as_object());
    if (vm.exception())
        return {};

    if (!status) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectSetPrototypeOfReturnedFalse);
        return {};
    }

    return object;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::is_extensible)
{
    auto argument = vm.argument(0);
    if (!argument.is_object())
        return Value(false);
    return Value(argument.as_object().is_extensible());
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::is_frozen)
{
    auto argument = vm.argument(0);
    if (!argument.is_object())
        return Value(true);
    return Value(argument.as_object().test_integrity_level(Object::IntegrityLevel::Frozen));
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::is_sealed)
{
    auto argument = vm.argument(0);
    if (!argument.is_object())
        return Value(true);
    return Value(argument.as_object().test_integrity_level(Object::IntegrityLevel::Sealed));
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::prevent_extensions)
{
    auto argument = vm.argument(0);
    if (!argument.is_object())
        return argument;
    auto status = argument.as_object().internal_prevent_extensions();
    if (vm.exception())
        return {};
    if (!status) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectPreventExtensionsReturnedFalse);
        return {};
    }
    return argument;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::freeze)
{
    auto argument = vm.argument(0);
    if (!argument.is_object())
        return argument;
    auto status = argument.as_object().set_integrity_level(Object::IntegrityLevel::Frozen);
    if (vm.exception())
        return {};
    if (!status) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectFreezeFailed);
        return {};
    }
    return argument;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::from_entries)
{
    auto iterable = require_object_coercible(global_object, vm.argument(0));
    if (vm.exception())
        return {};

    auto* object = Object::create(global_object, global_object.object_prototype());

    get_iterator_values(global_object, iterable, [&](Value iterator_value) {
        if (vm.exception())
            return IterationDecision::Break;
        if (!iterator_value.is_object()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, String::formatted("Iterator value {}", iterator_value.to_string_without_side_effects()));
            return IterationDecision::Break;
        }
        auto key = iterator_value.as_object().get(0);
        if (vm.exception())
            return IterationDecision::Break;
        auto value = iterator_value.as_object().get(1);
        if (vm.exception())
            return IterationDecision::Break;
        auto property_key = key.to_property_key(global_object);
        if (vm.exception())
            return IterationDecision::Break;
        object->create_data_property_or_throw(property_key, value);
        if (vm.exception())
            return IterationDecision::Break;
        return IterationDecision::Continue;
    });
    if (vm.exception())
        return {};
    return object;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::seal)
{
    auto argument = vm.argument(0);
    if (!argument.is_object())
        return argument;
    auto status = argument.as_object().set_integrity_level(Object::IntegrityLevel::Sealed);
    if (vm.exception())
        return {};
    if (!status) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectSealFailed);
        return {};
    }
    return argument;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::get_own_property_descriptor)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};
    auto key = vm.argument(1).to_property_key(global_object);
    if (vm.exception())
        return {};
    auto descriptor = object->internal_get_own_property(key);
    if (vm.exception())
        return {};
    return from_property_descriptor(global_object, descriptor);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::get_own_property_descriptors)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};
    auto own_keys = object->internal_own_property_keys();
    if (vm.exception())
        return {};
    auto* descriptors = Object::create(global_object, global_object.object_prototype());
    for (auto& key : own_keys) {
        auto property_name = PropertyName::from_value(global_object, key);

        auto desc = object->internal_get_own_property(property_name);
        if (vm.exception())
            return {};

        auto descriptor = from_property_descriptor(global_object, desc);

        if (!descriptor.is_undefined())
            descriptors->create_data_property_or_throw(property_name, descriptor);
    }

    return descriptors;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::define_property)
{
    if (!vm.argument(0).is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, vm.argument(0).to_string_without_side_effects());
        return {};
    }
    auto key = vm.argument(1).to_property_key(global_object);
    if (vm.exception())
        return {};
    auto descriptor = to_property_descriptor(global_object, vm.argument(2));
    if (vm.exception())
        return {};
    vm.argument(0).as_object().define_property_or_throw(key, descriptor);
    if (vm.exception())
        return {};
    return vm.argument(0);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::define_properties)
{
    auto object = vm.argument(0);
    auto properties = vm.argument(1);

    if (!object.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, "Object argument");
        return {};
    }

    return object.as_object().define_properties(properties);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::is)
{
    return Value(same_value(vm.argument(0), vm.argument(1)));
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::keys)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};
    auto name_list = object->enumerable_own_property_names(PropertyKind::Key);
    if (vm.exception())
        return {};
    return Array::create_from(global_object, name_list);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::values)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};
    auto name_list = object->enumerable_own_property_names(PropertyKind::Value);
    if (vm.exception())
        return {};
    return Array::create_from(global_object, name_list);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::entries)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};
    auto name_list = object->enumerable_own_property_names(PropertyKind::KeyAndValue);
    if (vm.exception())
        return {};
    return Array::create_from(global_object, name_list);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::create)
{
    auto proto = vm.argument(0);
    auto properties = vm.argument(1);

    if (!proto.is_object() && !proto.is_null()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectPrototypeWrongType);
        return {};
    }

    auto* object = Object::create(global_object, proto.is_null() ? nullptr : &proto.as_object());

    if (!properties.is_undefined()) {
        return object->define_properties(properties);
    }

    return object;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::has_own)
{
    auto* object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};

    auto key = vm.argument(1).to_property_key(global_object);
    if (vm.exception())
        return {};

    return Value(object->has_own_property(key));
}

JS_DEFINE_NATIVE_FUNCTION(ObjectConstructor::assign)
{
    auto* to = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};

    if (vm.argument_count() == 1)
        return to;

    for (size_t i = 1; i < vm.argument_count(); ++i) {
        auto next_source = vm.argument(i);

        if (next_source.is_nullish())
            continue;

        auto from = next_source.to_object(global_object);
        VERIFY(!vm.exception());

        auto keys = from->internal_own_property_keys();
        if (vm.exception())
            return {};

        for (auto& next_key : keys) {
            auto property_name = PropertyName::from_value(global_object, next_key);

            auto desc = from->internal_get_own_property(property_name);
            if (vm.exception())
                return {};

            if (!desc.has_value() || !*desc->enumerable)
                continue;

            auto prop_value = from->get(property_name);
            if (vm.exception())
                return {};

            to->set(property_name, prop_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
        }
    }

    return to;
}

}
