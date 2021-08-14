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
#include <libjs/runtime/Map.h>
#include <libjs/runtime/MapConstructor.h>

namespace JS {

MapConstructor::MapConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Map.as_string(), *global_object.function_prototype())
{
}

void MapConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.map_prototype(), 0);

    define_native_accessor(*vm.well_known_symbol_species(), symbol_species_getter, {}, Attribute::Configurable);

    define_direct_property(vm.names.length, Value(0), Attribute::Configurable);
}

MapConstructor::~MapConstructor()
{
}

Value MapConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.Map);
    return {};
}

Value MapConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto* map = ordinary_create_from_constructor<Map>(global_object, new_target, &GlobalObject::map_prototype);
    if (vm.exception())
        return {};

    if (vm.argument(0).is_nullish())
        return map;

    auto adder = map->get(vm.names.set);
    if (vm.exception())
        return {};
    if (!adder.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, "'set' property of Map");
        return {};
    }
    get_iterator_values(global_object, vm.argument(0), [&](Value iterator_value) {
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
        (void)vm.call(adder.as_function(), Value(map), key, value);
        return vm.exception() ? IterationDecision::Break : IterationDecision::Continue;
    });
    if (vm.exception())
        return {};
    return map;
}

JS_DEFINE_NATIVE_GETTER(MapConstructor::symbol_species_getter)
{
    return vm.this_value(global_object);
}

}
