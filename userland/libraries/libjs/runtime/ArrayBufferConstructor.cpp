/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/ArrayBuffer.h>
#include <libjs/runtime/ArrayBufferConstructor.h>
#include <libjs/runtime/DataView.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/TypedArray.h>

namespace JS {

ArrayBufferConstructor::ArrayBufferConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.ArrayBuffer.as_string(), *global_object.function_prototype())
{
}

void ArrayBufferConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.array_buffer_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.isView, is_view, 1, attr);
    
    define_native_accessor(*vm.well_known_symbol_species(), symbol_species_getter, {}, Attribute::Configurable);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

ArrayBufferConstructor::~ArrayBufferConstructor()
{
}

Value ArrayBufferConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.ArrayBuffer);
    return {};
}

Value ArrayBufferConstructor::construct(FunctionObject&)
{
    auto& vm = this->vm();
    auto byte_length = vm.argument(0).to_index(global_object());
    if (vm.exception()) {
        if (vm.exception()->value().is_object() && is<RangeError>(vm.exception()->value().as_object())) {

            vm.clear_exception();
            vm.throw_exception<RangeError>(global_object(), ErrorType::InvalidLength, "array buffer");
        }
        return {};
    }
    return ArrayBuffer::create(global_object(), byte_length);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayBufferConstructor::is_view)
{
    auto arg = vm.argument(0);
    if (!arg.is_object())
        return Value(false);
    if (arg.as_object().is_typed_array())
        return Value(true);
    if (is<DataView>(arg.as_object()))
        return Value(true);
    return Value(false);
}

JS_DEFINE_NATIVE_GETTER(ArrayBufferConstructor::symbol_species_getter)
{
    return vm.this_value(global_object);
}

}
