/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <base/Function.h>
#include <base/TypeCasts.h>
#include <libjs/runtime/BooleanPrototype.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

BooleanPrototype::BooleanPrototype(GlobalObject& global_object)
    : BooleanObject(false, *global_object.object_prototype())
{
}

void BooleanPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    BooleanObject::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.toString, to_string, 0, attr);
    define_native_function(vm.names.valueOf, value_of, 0, attr);
}

BooleanPrototype::~BooleanPrototype()
{
}

JS_DEFINE_NATIVE_FUNCTION(BooleanPrototype::to_string)
{
    auto this_value = vm.this_value(global_object);
    if (this_value.is_boolean())
        return js_string(vm, this_value.as_bool() ? "true" : "false");
    if (!this_value.is_object() || !is<BooleanObject>(this_value.as_object())) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Boolean");
        return {};
    }

    bool bool_value = static_cast<const BooleanObject&>(this_value.as_object()).value_of().as_bool();
    return js_string(vm, bool_value ? "true" : "false");
}

JS_DEFINE_NATIVE_FUNCTION(BooleanPrototype::value_of)
{
    auto this_value = vm.this_value(global_object);
    if (this_value.is_boolean())
        return this_value;
    if (!this_value.is_object() || !is<BooleanObject>(this_value.as_object())) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Boolean");
        return {};
    }

    return static_cast<const BooleanObject&>(this_value.as_object()).value_of();
}
}
