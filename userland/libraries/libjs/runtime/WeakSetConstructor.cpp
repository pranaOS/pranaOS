/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/IteratorOperations.h>
#include <libjs/runtime/WeakSet.h>
#include <libjs/runtime/WeakSetConstructor.h>

namespace JS {

WeakSetConstructor::WeakSetConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.WeakSet.as_string(), *global_object.function_prototype())
{
}

void WeakSetConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.weak_set_prototype(), 0);

    define_direct_property(vm.names.length, Value(0), Attribute::Configurable);
}

WeakSetConstructor::~WeakSetConstructor()
{
}

Value WeakSetConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.WeakSet);
    return {};
}

Value WeakSetConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto* weak_set = ordinary_create_from_constructor<WeakSet>(global_object, new_target, &GlobalObject::weak_set_prototype);
    if (vm.exception())
        return {};

    if (vm.argument(0).is_nullish())
        return weak_set;

    auto adder = weak_set->get(vm.names.add);
    if (vm.exception())
        return {};
    if (!adder.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, "'add' property of WeakSet");
        return {};
    }
    get_iterator_values(global_object, vm.argument(0), [&](Value iterator_value) {
        if (vm.exception())
            return IterationDecision::Break;
        (void)vm.call(adder.as_function(), Value(weak_set), iterator_value);
        return vm.exception() ? IterationDecision::Break : IterationDecision::Continue;
    });
    if (vm.exception())
        return {};
    return weak_set;
}

}
