/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Function.h>
#include <base/TypeCasts.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/PrimitiveString.h>
#include <libjs/runtime/SymbolObject.h>
#include <libjs/runtime/SymbolPrototype.h>
#include <libjs/runtime/Value.h>

namespace JS {

SymbolPrototype::SymbolPrototype(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void SymbolPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.toString, to_string, 0, attr);
    define_native_function(vm.names.valueOf, value_of, 0, attr);
    define_native_accessor(vm.names.description, description_getter, {}, Attribute::Configurable);
    define_native_function(*vm.well_known_symbol_to_primitive(), symbol_to_primitive, 1, Attribute::Configurable);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(global_object.heap(), "Symbol"), Attribute::Configurable);
}

SymbolPrototype::~SymbolPrototype()
{
}

static Value this_symbol_value(GlobalObject& global_object, Value value)
{
    if (value.is_symbol())
        return value;
    if (value.is_object() && is<SymbolObject>(value.as_object()))
        return static_cast<SymbolObject&>(value.as_object()).value_of();
    auto& vm = global_object.vm();
    vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Symbol");
    return {};
}

JS_DEFINE_NATIVE_GETTER(SymbolPrototype::description_getter)
{
    auto symbol_value = this_symbol_value(global_object, vm.this_value(global_object));
    if (vm.exception())
        return {};
    auto& description = symbol_value.as_symbol().raw_description();
    if (!description.has_value())
        return js_undefined();
    return js_string(vm, *description);
}

JS_DEFINE_NATIVE_FUNCTION(SymbolPrototype::to_string)
{
    auto symbol_value = this_symbol_value(global_object, vm.this_value(global_object));
    if (vm.exception())
        return {};
    return js_string(vm, symbol_value.as_symbol().to_string());
}

JS_DEFINE_NATIVE_FUNCTION(SymbolPrototype::value_of)
{
    return this_symbol_value(global_object, vm.this_value(global_object));
}

JS_DEFINE_NATIVE_FUNCTION(SymbolPrototype::symbol_to_primitive)
{
    return this_symbol_value(global_object, vm.this_value(global_object));
}

}
