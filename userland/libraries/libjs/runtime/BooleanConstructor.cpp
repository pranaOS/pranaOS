/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/BooleanConstructor.h>
#include <libjs/runtime/BooleanObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

BooleanConstructor::BooleanConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Boolean.as_string(), *global_object.function_prototype())
{
}

void BooleanConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.boolean_prototype(), 0);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

BooleanConstructor::~BooleanConstructor()
{
}

Value BooleanConstructor::call()
{
    auto& vm = this->vm();

    auto b = vm.argument(0).to_boolean();
    return Value(b);
}

Value BooleanConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto b = vm.argument(0).to_boolean();
    return ordinary_create_from_constructor<BooleanObject>(global_object, new_target, &GlobalObject::boolean_prototype, b);
}

}
