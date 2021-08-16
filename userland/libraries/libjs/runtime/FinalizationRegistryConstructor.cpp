/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FinalizationRegistry.h>
#include <libjs/runtime/FinalizationRegistryConstructor.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

FinalizationRegistryConstructor::FinalizationRegistryConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.FinalizationRegistry.as_string(), *global_object.function_prototype())
{
}

void FinalizationRegistryConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.finalization_registry_prototype(), 0);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

FinalizationRegistryConstructor::~FinalizationRegistryConstructor()
{
}

Value FinalizationRegistryConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.FinalizationRegistry);
    return {};
}

Value FinalizationRegistryConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto cleanup_callback = vm.argument(0);
    if (!cleanup_callback.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, cleanup_callback.to_string_without_side_effects());
        return {};
    }
    return ordinary_create_from_constructor<FinalizationRegistry>(global_object, new_target, &GlobalObject::finalization_registry_prototype, cleanup_callback.as_function());
}

}
