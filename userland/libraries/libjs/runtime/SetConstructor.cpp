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
#include <libjs/runtime/Set.h>
#include <libjs/runtime/SetConstructor.h>

namespace JS {

SetConstructor::SetConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Set.as_string(), *global_object.function_prototype())
{
}

void SetConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.set_prototype(), 0);

    define_native_accessor(*vm.well_known_symbol_species(), symbol_species_getter, {}, Attribute::Configurable);

    define_direct_property(vm.names.length, Value(0), Attribute::Configurable);
}

SetConstructor::~SetConstructor()
{
}

Value SetConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.Set);
    return {};
}

Value SetConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto* set = ordinary_create_from_constructor<Set>(global_object, new_target, &GlobalObject::set_prototype);
    if (vm.exception())
        return {};

    if (vm.argument(0).is_nullish())
        return set;

    auto adder = set->get(vm.names.add);
    if (vm.exception())
        return {};
    if (!adder.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, "'add' property of Set");
        return {};
    }
    get_iterator_values(global_object, vm.argument(0), [&](Value iterator_value) {
        if (vm.exception())
            return IterationDecision::Break;
        (void)vm.call(adder.as_function(), Value(set), iterator_value);
        return vm.exception() ? IterationDecision::Break : IterationDecision::Continue;
    });
    if (vm.exception())
        return {};
    return set;
}

JS_DEFINE_NATIVE_GETTER(SetConstructor::symbol_species_getter)
{
    return vm.this_value(global_object);
}

}
