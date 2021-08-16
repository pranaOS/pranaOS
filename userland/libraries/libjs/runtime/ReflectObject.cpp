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
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/NativeFunction.h>
#include <libjs/runtime/ReflectObject.h>

namespace JS {

ReflectObject::ReflectObject(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void ReflectObject::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.apply, apply, 3, attr);
    define_native_function(vm.names.construct, construct, 2, attr);
    define_native_function(vm.names.defineProperty, define_property, 3, attr);
    define_native_function(vm.names.deleteProperty, delete_property, 2, attr);
    define_native_function(vm.names.get, get, 2, attr);
    define_native_function(vm.names.getOwnPropertyDescriptor, get_own_property_descriptor, 2, attr);
    define_native_function(vm.names.getPrototypeOf, get_prototype_of, 1, attr);
    define_native_function(vm.names.has, has, 2, attr);
    define_native_function(vm.names.isExtensible, is_extensible, 1, attr);
    define_native_function(vm.names.ownKeys, own_keys, 1, attr);
    define_native_function(vm.names.preventExtensions, prevent_extensions, 1, attr);
    define_native_function(vm.names.set, set, 3, attr);
    define_native_function(vm.names.setPrototypeOf, set_prototype_of, 2, attr);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(vm, vm.names.Reflect.as_string()), Attribute::Configurable);
}

ReflectObject::~ReflectObject()
{
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::apply)
{
    auto target = vm.argument(0);
    auto this_argument = vm.argument(1);
    auto arguments_list = vm.argument(2);

    if (!target.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, target.to_string_without_side_effects());
        return {};
    }

    auto args = create_list_from_array_like(global_object, arguments_list);
    if (vm.exception())
        return {};

    return vm.call(target.as_function(), this_argument, move(args));
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::construct)
{
    auto target = vm.argument(0);
    auto arguments_list = vm.argument(1);
    auto new_target = vm.argument(2);

    if (!target.is_constructor()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAConstructor, target.to_string_without_side_effects());
        return {};
    }

    if (vm.argument_count() < 3) {
        new_target = target;
    }
    else if (!new_target.is_constructor()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAConstructor, new_target.to_string_without_side_effects());
        return {};
    }


    auto args = create_list_from_array_like(global_object, arguments_list);
    if (vm.exception())
        return {};

    return vm.construct(target.as_function(), new_target.as_function(), move(args));
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::define_property)
{
    auto target = vm.argument(0);
    auto property_key = vm.argument(1);
    auto attributes = vm.argument(2);

    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    auto key = property_key.to_property_key(global_object);
    if (vm.exception())
        return {};

    auto descriptor = to_property_descriptor(global_object, attributes);
    if (vm.exception())
        return {};

    return Value(target.as_object().internal_define_own_property(key, descriptor));
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::delete_property)
{
    auto target = vm.argument(0);
    auto property_key = vm.argument(1);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Let key be ? ToPropertyKey(propertyKey).
    auto key = property_key.to_property_key(global_object);
    if (vm.exception())
        return {};

    // 3. Return ? target.[[Delete]](key).
    return Value(target.as_object().internal_delete(key));
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::get)
{
    auto target = vm.argument(0);
    auto property_key = vm.argument(1);
    auto receiver = vm.argument(2);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Let key be ? ToPropertyKey(propertyKey).
    auto key = property_key.to_property_key(global_object);
    if (vm.exception())
        return {};

    // 3. If receiver is not present, then
    if (vm.argument_count() < 3) {
        // a. Set receiver to target.
        receiver = target;
    }

    // 4. Return ? target.[[Get]](key, receiver).
    return target.as_object().internal_get(key, receiver);
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::get_own_property_descriptor)
{
    auto target = vm.argument(0);
    auto property_key = vm.argument(1);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Let key be ? ToPropertyKey(propertyKey).
    auto key = property_key.to_property_key(global_object);
    if (vm.exception())
        return {};

    // 3. Let desc be ? target.[[GetOwnProperty]](key).
    auto descriptor = target.as_object().internal_get_own_property(key);
    if (vm.exception())
        return {};

    // 4. Return FromPropertyDescriptor(desc).
    return from_property_descriptor(global_object, descriptor);
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::get_prototype_of)
{
    auto target = vm.argument(0);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Return ? target.[[GetPrototypeOf]]().
    return target.as_object().internal_get_prototype_of();
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::has)
{
    auto target = vm.argument(0);
    auto property_key = vm.argument(1);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Let key be ? ToPropertyKey(propertyKey).
    auto key = property_key.to_property_key(global_object);
    if (vm.exception())
        return {};

    // 3. Return ? target.[[HasProperty]](key).
    return Value(target.as_object().internal_has_property(key));
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::is_extensible)
{
    auto target = vm.argument(0);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Return ? target.[[IsExtensible]]().
    return Value(target.as_object().internal_is_extensible());
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::own_keys)
{
    auto target = vm.argument(0);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Let keys be ? target.[[OwnPropertyKeys]]().
    auto keys = target.as_object().internal_own_property_keys();
    if (vm.exception())
        return {};

    // 3. Return CreateArrayFromList(keys).
    return Array::create_from(global_object, keys);
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::prevent_extensions)
{
    auto target = vm.argument(0);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Return ? target.[[PreventExtensions]]().
    return Value(target.as_object().internal_prevent_extensions());
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::set)
{
    auto target = vm.argument(0);
    auto property_key = vm.argument(1);
    auto value = vm.argument(2);
    auto receiver = vm.argument(3);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. Let key be ? ToPropertyKey(propertyKey).
    auto key = property_key.to_property_key(global_object);
    if (vm.exception())
        return {};

    // 3. If receiver is not present, then
    if (vm.argument_count() < 4) {
        // a. Set receiver to target.
        receiver = target;
    }

    // 4. Return ? target.[[Set]](key, V, receiver).
    return Value(target.as_object().internal_set(key, value, receiver));
}

JS_DEFINE_NATIVE_FUNCTION(ReflectObject::set_prototype_of)
{
    auto target = vm.argument(0);
    auto proto = vm.argument(1);

    // 1. If Type(target) is not Object, throw a TypeError exception.
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }

    // 2. If Type(proto) is not Object and proto is not null, throw a TypeError exception.
    if (!proto.is_object() && !proto.is_null()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectPrototypeWrongType);
        return {};
    }

    // 3. Return ? target.[[SetPrototypeOf]](proto).
    return Value(target.as_object().internal_set_prototype_of(proto.is_null() ? nullptr : &proto.as_object()));
}

}
