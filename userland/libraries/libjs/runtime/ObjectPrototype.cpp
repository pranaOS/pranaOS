/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Function.h>
#include <base/String.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Accessor.h>
#include <libjs/runtime/BooleanObject.h>
#include <libjs/runtime/Date.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/NumberObject.h>
#include <libjs/runtime/ObjectPrototype.h>
#include <libjs/runtime/RegExpObject.h>
#include <libjs/runtime/StringObject.h>
#include <libjs/runtime/Value.h>

namespace JS {

ObjectPrototype::ObjectPrototype(GlobalObject& global_object)
    : Object(Object::ConstructWithoutPrototypeTag::Tag, global_object)
{
}

void ObjectPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.hasOwnProperty, has_own_property, 1, attr);
    define_native_function(vm.names.toString, to_string, 0, attr);
    define_native_function(vm.names.toLocaleString, to_locale_string, 0, attr);
    define_native_function(vm.names.valueOf, value_of, 0, attr);
    define_native_function(vm.names.propertyIsEnumerable, property_is_enumerable, 1, attr);
    define_native_function(vm.names.isPrototypeOf, is_prototype_of, 1, attr);

    define_native_function(vm.names.__defineGetter__, define_getter, 2, attr);
    define_native_function(vm.names.__defineSetter__, define_setter, 2, attr);
    define_native_function(vm.names.__lookupGetter__, lookup_getter, 1, attr);
    define_native_function(vm.names.__lookupSetter__, lookup_setter, 1, attr);
    define_native_accessor(vm.names.__proto__, proto_getter, proto_setter, Attribute::Configurable);
}

ObjectPrototype::~ObjectPrototype()
{
}

bool ObjectPrototype::internal_set_prototype_of(Object* prototype)
{
    return set_immutable_prototype(prototype);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::has_own_property)
{
    auto property_key = vm.argument(0).to_property_key(global_object);
    if (vm.exception())
        return {};
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    return Value(this_object->has_own_property(property_key));
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::to_string)
{
    auto this_value = vm.this_value(global_object);

    if (this_value.is_undefined())
        return js_string(vm, "[object Undefined]");


    if (this_value.is_null())
        return js_string(vm, "[object Null]");

    // 3. Let O be ! ToObject(this value).
    auto* object = this_value.to_object(global_object);
    VERIFY(object);

    // 4. Let isArray be ? IsArray(O).
    auto is_array = Value(object).is_array(global_object);
    if (vm.exception())
        return {};

    String builtin_tag;

    // 5. If isArray is true, let builtinTag be "Array".
    if (is_array)
        builtin_tag = "Array";
    // 6. Else if O has a [[ParameterMap]] internal slot, let builtinTag be "Arguments".
    else if (object->has_parameter_map())
        builtin_tag = "Arguments";
    // 7. Else if O has a [[Call]] internal method, let builtinTag be "Function".
    else if (object->is_function())
        builtin_tag = "Function";
    // 8. Else if O has an [[ErrorData]] internal slot, let builtinTag be "Error".
    else if (is<Error>(object))
        builtin_tag = "Error";
    // 9. Else if O has a [[BooleanData]] internal slot, let builtinTag be "Boolean".
    else if (is<BooleanObject>(object))
        builtin_tag = "Boolean";
    // 10. Else if O has a [[NumberData]] internal slot, let builtinTag be "Number".
    else if (is<NumberObject>(object))
        builtin_tag = "Number";
    // 11. Else if O has a [[StringData]] internal slot, let builtinTag be "String".
    else if (is<StringObject>(object))
        builtin_tag = "String";
    // 12. Else if O has a [[DateValue]] internal slot, let builtinTag be "Date".
    else if (is<Date>(object))
        builtin_tag = "Date";
    // 13. Else if O has a [[RegExpMatcher]] internal slot, let builtinTag be "RegExp".
    else if (is<RegExpObject>(object))
        builtin_tag = "RegExp";
    // 14. Else, let builtinTag be "Object".
    else
        builtin_tag = "Object";

    // 15. Let tag be ? Get(O, @@toStringTag).
    auto to_string_tag = object->get(*vm.well_known_symbol_to_string_tag());
    if (vm.exception())
        return {};

    // Optimization: Instead of creating another PrimitiveString from builtin_tag, we separate tag and to_string_tag and add an additional branch to step 16.
    String tag;

    // 16. If Type(tag) is not String, set tag to builtinTag.
    if (!to_string_tag.is_string())
        tag = move(builtin_tag);
    else
        tag = to_string_tag.as_string().string();

    // 17. Return the string-concatenation of "[object ", tag, and "]".
    return js_string(vm, String::formatted("[object {}]", tag));
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::to_locale_string)
{
    auto this_value = vm.this_value(global_object);
    return this_value.invoke(global_object, vm.names.toString);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::value_of)
{
    return vm.this_value(global_object).to_object(global_object);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::property_is_enumerable)
{
    // 1. Let P be ? ToPropertyKey(V).
    auto property_key = vm.argument(0).to_property_key(global_object);
    if (vm.exception())
        return {};
    // 2. Let O be ? ToObject(this value).
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    // 3. Let desc be ? O.[[GetOwnProperty]](P).
    auto property_descriptor = this_object->internal_get_own_property(property_key);
    if (vm.exception())
        return {};
    // 4. If desc is undefined, return false.
    if (!property_descriptor.has_value())
        return Value(false);
    // 5. Return desc.[[Enumerable]].
    return Value(*property_descriptor->enumerable);
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::is_prototype_of)
{
    auto object_argument = vm.argument(0);
    if (!object_argument.is_object())
        return Value(false);
    auto* object = &object_argument.as_object();
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    for (;;) {
        object = object->internal_get_prototype_of();
        if (!object)
            return Value(false);
        if (same_value(this_object, object))
            return Value(true);
    }
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::define_getter)
{
    auto object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto getter = vm.argument(1);
    if (!getter.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, getter.to_string_without_side_effects());
        return {};
    }

    auto descriptor = PropertyDescriptor { .get = &getter.as_function(), .enumerable = true, .configurable = true };

    auto key = vm.argument(0).to_property_key(global_object);
    if (vm.exception())
        return {};

    object->define_property_or_throw(key, descriptor);
    if (vm.exception())
        return {};

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::define_setter)
{
    auto object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto setter = vm.argument(1);
    if (!setter.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, setter.to_string_without_side_effects());
        return {};
    }

    auto descriptor = PropertyDescriptor { .set = &setter.as_function(), .enumerable = true, .configurable = true };

    auto key = vm.argument(0).to_property_key(global_object);
    if (vm.exception())
        return {};

    object->define_property_or_throw(key, descriptor);
    if (vm.exception())
        return {};

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::lookup_getter)
{
    auto object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto key = vm.argument(0).to_property_key(global_object);
    if (vm.exception())
        return {};

    while (object) {
        auto desc = object->internal_get_own_property(key);
        if (vm.exception())
            return {};
        if (desc.has_value()) {
            if (desc->is_accessor_descriptor())
                return *desc->get ?: js_undefined();
            return js_undefined();
        }
        object = object->internal_get_prototype_of();
        if (vm.exception())
            return {};
    }

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::lookup_setter)
{
    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto key = vm.argument(0).to_property_key(global_object);
    if (vm.exception())
        return {};

    while (object) {
        auto desc = object->internal_get_own_property(key);
        if (vm.exception())
            return {};
        if (desc.has_value()) {
            if (desc->is_accessor_descriptor())
                return *desc->set ?: js_undefined();
            return js_undefined();
        }
        object = object->internal_get_prototype_of();
        if (vm.exception())
            return {};
    }

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::proto_getter)
{
    auto object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};
    auto proto = object->internal_get_prototype_of();
    if (vm.exception())
        return {};
    return proto;
}

JS_DEFINE_NATIVE_FUNCTION(ObjectPrototype::proto_setter)
{
    auto object = require_object_coercible(global_object, vm.this_value(global_object));
    if (vm.exception())
        return {};

    auto proto = vm.argument(0);
    if (!proto.is_object() && !proto.is_null())
        return js_undefined();

    if (!object.is_object())
        return js_undefined();

    auto status = object.as_object().internal_set_prototype_of(proto.is_object() ? &proto.as_object() : nullptr);
    if (vm.exception())
        return {};
    if (!status) {

        vm.throw_exception<TypeError>(global_object, ErrorType::ObjectSetPrototypeOfReturnedFalse);
        return {};
    }
    return js_undefined();
}

}
