/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/WeakRef.h>
#include <libjs/runtime/WeakRefConstructor.h>

namespace JS {

WeakRefConstructor::WeakRefConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.WeakRef.as_string(), *global_object.function_prototype())
{
}

void WeakRefConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.weak_ref_prototype(), 0);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

WeakRefConstructor::~WeakRefConstructor()
{
}

Value WeakRefConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.WeakRef);
    return {};
}

Value WeakRefConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto target = vm.argument(0);
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, target.to_string_without_side_effects());
        return {};
    }
    return ordinary_create_from_constructor<WeakRef>(global_object, new_target, &GlobalObject::weak_ref_prototype, &target.as_object());
}

}
