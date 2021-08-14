/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/StringBuilder.h>
#include <base/Utf16View.h>
#include <base/Utf32View.h>
#include <libjs/Runtime/AbstractOperations.h>
#include <libjs/Runtime/Array.h>
#include <libjs/Runtime/Error.h>
#include <libjs/Runtime/GlobalObject.h>
#include <libjs/Runtime/StringConstructor.h>
#include <libjs/Runtime/StringObject.h>
#include <libjs/Runtime/Utf16String.h>

namespace JS {

StringConstructor::StringConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.String.as_string(), *global_object.function_prototype())
{
}

void StringConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.string_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.raw, raw, 1, attr);
    define_native_function(vm.names.fromCharCode, from_char_code, 1, attr);
    define_native_function(vm.names.fromCodePoint, from_code_point, 1, attr);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

StringConstructor::~StringConstructor()
{
}

Value StringConstructor::call()
{
    if (!vm().argument_count())
        return js_string(heap(), "");
    if (vm().argument(0).is_symbol())
        return js_string(heap(), vm().argument(0).as_symbol().to_string());
    auto* string = vm().argument(0).to_primitive_string(global_object());
    if (vm().exception())
        return {};
    return string;
}

Value StringConstructor::construct(FunctionObject& new_target)
{
    auto& vm = global_object().vm();

    PrimitiveString* primitive_string;
    if (!vm.argument_count())
        primitive_string = js_string(vm, "");
    else
        primitive_string = vm.argument(0).to_primitive_string(global_object());
    if (!primitive_string)
        return {};
    auto* prototype = get_prototype_from_constructor(global_object(), new_target, &GlobalObject::string_prototype);
    if (vm.exception())
        return {};
    return StringObject::create(global_object(), *primitive_string, *prototype);
}

JS_DEFINE_NATIVE_FUNCTION(StringConstructor::raw)
{
    auto* cooked = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};

    auto raw_value = cooked->get(vm.names.raw);
    if (vm.exception())
        return {};

    auto* raw = raw_value.to_object(global_object);
    if (vm.exception())
        return {};

    auto literal_segments = length_of_array_like(global_object, *raw);
    if (vm.exception())
        return {};

    if (literal_segments == 0)
        return js_string(vm, "");

    const auto number_of_substituions = vm.argument_count() - 1;

    StringBuilder builder;
    for (size_t i = 0; i < literal_segments; ++i) {
        auto next_key = String::number(i);
        auto next_segment_value = raw->get(next_key);
        if (vm.exception())
            return {};
        auto next_segment = next_segment_value.to_string(global_object);
        if (vm.exception())
            return {};

        builder.append(next_segment);

        if (i + 1 == literal_segments)
            break;

        if (i < number_of_substituions) {
            auto next = vm.argument(i + 1);
            auto next_sub = next.to_string(global_object);
            if (vm.exception())
                return {};
            builder.append(next_sub);
        }
    }
    return js_string(vm, builder.build());
}


JS_DEFINE_NATIVE_FUNCTION(StringConstructor::from_char_code)
{
    Vector<u16> string;
    string.ensure_capacity(vm.argument_count());

    for (size_t i = 0; i < vm.argument_count(); ++i) {
        auto code_unit = vm.argument(i).to_u16(global_object);
        if (vm.exception())
            return {};
        string.append(code_unit);
    }

    return js_string(vm, Utf16String(move(string)));
}

JS_DEFINE_NATIVE_FUNCTION(StringConstructor::from_code_point)
{
    Vector<u16> string;
    string.ensure_capacity(vm.argument_count()); 

    for (size_t i = 0; i < vm.argument_count(); ++i) {
        auto next_code_point = vm.argument(i).to_number(global_object);
        if (vm.exception())
            return {};
        if (!next_code_point.is_integral_number()) {
            vm.throw_exception<RangeError>(global_object, ErrorType::InvalidCodePoint, next_code_point.to_string_without_side_effects());
            return {};
        }
        auto code_point = next_code_point.to_i32(global_object);
        if (code_point < 0 || code_point > 0x10FFFF) {
            vm.throw_exception<RangeError>(global_object, ErrorType::InvalidCodePoint, next_code_point.to_string_without_side_effects());
            return {};
        }

        AK::code_point_to_utf16(string, static_cast<u32>(code_point));
    }

    return js_string(vm, Utf16String(move(string)));
}

}
