/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/AggregateError.h>
#include <libjs/runtime/AggregateErrorConstructor.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/ErrorConstructor.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/IteratorOperations.h>

namespace JS {

AggregateErrorConstructor::AggregateErrorConstructor(GlobalObject& global_object)
    : NativeFunction(*static_cast<Object*>(global_object.error_constructor()))
{
}

void AggregateErrorConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.aggregate_error_prototype(), 0);

    define_direct_property(vm.names.length, Value(2), Attribute::Configurable);
}

Value AggregateErrorConstructor::call()
{
    return construct(*this);
}

Value AggregateErrorConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto* aggregate_error = ordinary_create_from_constructor<AggregateError>(global_object, new_target, &GlobalObject::aggregate_error_prototype);
    if (vm.exception())
        return {};

    if (!vm.argument(1).is_undefined()) {
        auto message = vm.argument(1).to_string(global_object);
        if (vm.exception())
            return {};
        aggregate_error->create_non_enumerable_data_property_or_throw(vm.names.message, js_string(vm, message));
    }

    aggregate_error->install_error_cause(vm.argument(2));
    if (vm.exception())
        return {};

    auto errors_list = iterable_to_list(global_object, vm.argument(0));
    if (vm.exception())
        return {};

    aggregate_error->define_property_or_throw(vm.names.errors, { .value = Array::create_from(global_object, errors_list), .writable = true, .enumerable = false, .configurable = true });

    return aggregate_error;
}

}

