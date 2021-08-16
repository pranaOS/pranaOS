/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/SymbolConstructor.h>

namespace JS {

SymbolConstructor::SymbolConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Symbol.as_string(), *global_object.function_prototype())
{
}

void SymbolConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.symbol_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.for_, for_, 1, attr);
    define_native_function(vm.names.keyFor, key_for, 1, attr);

#define __JS_ENUMERATE(SymbolName, snake_name) \
    define_direct_property(vm.names.SymbolName, vm.well_known_symbol_##snake_name(), 0);
    JS_ENUMERATE_WELL_KNOWN_SYMBOLS
#undef __JS_ENUMERATE

    define_direct_property(vm.names.length, Value(0), Attribute::Configurable);
}

SymbolConstructor::~SymbolConstructor()
{
}

Value SymbolConstructor::call()
{
    if (vm().argument(0).is_undefined())
        return js_symbol(heap(), {}, false);
    return js_symbol(heap(), vm().argument(0).to_string(global_object()), false);
}

Value SymbolConstructor::construct(FunctionObject&)
{
    vm().throw_exception<TypeError>(global_object(), ErrorType::NotAConstructor, "Symbol");
    return {};
}

JS_DEFINE_NATIVE_FUNCTION(SymbolConstructor::for_)
{
    String description = vm.argument(0).to_string(global_object);
    return global_object.vm().get_global_symbol(description);
}

JS_DEFINE_NATIVE_FUNCTION(SymbolConstructor::key_for)
{
    auto argument = vm.argument(0);
    if (!argument.is_symbol()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotASymbol, argument.to_string_without_side_effects());
        return {};
    }

    auto& symbol = argument.as_symbol();
    if (symbol.is_global())
        return js_string(vm, symbol.description());

    return js_undefined();
}

}
