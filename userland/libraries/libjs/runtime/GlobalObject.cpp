/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


// includes
#include <base/CharacterTypes.h>
#include <base/Hex.h>
#include <base/Platform.h>
#include <base/Utf8View.h>
#include <libjs/Console.h>
#include <libjs/heap/DeferGC.h>
#include <libjs/Interpreter.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/AggregateErrorConstructor.h>
#include <libjs/runtime/AggregateErrorPrototype.h>
#include <libjs/runtime/ArrayBufferConstructor.h>
#include <libjs/runtime/ArrayBufferPrototype.h>
#include <libjs/runtime/ArrayConstructor.h>
#include <libjs/runtime/ArrayIteratorPrototype.h>
#include <libjs/runtime/ArrayPrototype.h>
#include <libjs/runtime/AtomicsObject.h>
#include <libjs/runtime/BigIntConstructor.h>
#include <libjs/runtime/BigIntPrototype.h>
#include <LibJS/runtime/BooleanConstructor.h>
#include <LibJS/runtime/BooleanPrototype.h>
#include <libjs/runtime/ConsoleObject.h>
#include <libjs/runtime/DataViewConstructor.h>
#include <libjs/runtime/DataViewPrototype.h>
#include <libjs/runtime/DateConstructor.h>
#include <libjs/runtime/DatePrototype.h>
#include <libjs/runtime/ErrorConstructor.h>
#include <libjs/runtime/ErrorPrototype.h>
#include <libjs/runtime/FinalizationRegistryConstructor.h>
#include <libjs/runtime/FinalizationRegistryPrototype.h>
#include <libjs/runtime/FunctionConstructor.h>
#include <libjs/runtime/FunctionPrototype.h>
#include <libjs/runtime/GeneratorFunctionConstructor.h>
#include <LibJS/runtime/GeneratorFunctionPrototype.h>
#include <libjs/runtime/GeneratorObjectPrototype.h>
#include <libjs/runtime/GlobalEnvironment.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Intl/Intl.h>
#include <libjs/runtime/IteratorPrototype.h>
#include <libjs/runtime/JSONObject.h>
#include <libjs/runtime/MapConstructor.h>
#include <libjs/runtime/MapIteratorPrototype.h>
#include <libjs/runtime/MapPrototype.h>
#include <LibJS/runtime/MathObject.h>
#include <libjs/runtime/NativeFunction.h>
#include <libjs/runtime/NumberConstructor.h>
#include <libjs/runtime/NumberPrototype.h>
#include <LibJS/runtime/Object.h>
#include <libjs/Runtime/ObjectConstructor.h>
#include <libjs/runtime/ObjectPrototype.h>
#include <libjs/runtime/PromiseConstructor.h>
#include <libjs/runtime/PromisePrototype.h>
#include <libjs/runtime/ProxyConstructor.h>
#include <libjs/runtime/ReflectObject.h>
#include <libjs/runtime/RegExpConstructor.h>
#include <libjs/runtime/RegExpPrototype.h>
#include <libjs/runtime/RegExpStringIteratorPrototype.h>
#include <libjs/runtime/SetConstructor.h>
#include <libjs/runtime/SetIteratorPrototype.h>
#include <libjs/runtime/SetPrototype.h>
#include <libjs/runtime/Shape.h>
#include <libjs/runtime/StringConstructor.h>
#include <libjs/runtime/StringIteratorPrototype.h>
#include <libjs/runtime/StringPrototype.h>
#include <libjs/runtime/SymbolConstructor.h>
#include <libjs/runtime/SymbolPrototype.h>
#include <libjs/runtime/temporal/CalendarConstructor.h>
#include <libjs/runtime/temporal/CalendarPrototype.h>
#include <libjs/runtime/temporal/DurationConstructor.h>
#include <libjs/runtime/temporal/DurationPrototype.h>
#include <libjs/runtime/temporal/InstantConstructor.h>
#include <libjs/runtime/temporal/InstantPrototype.h>
#include <libjs/runtime/temporal/PlainDateConstructor.h>
#include <libjs/runtime/temporal/PlainDatePrototype.h>
#include <libjs/runtime/temporal/PlainDateTimeConstructor.h>
#include <libjs/runtime/temporal/PlainDateTimePrototype.h>
#include <libjs/runtime/temporal/PlainTimeConstructor.h>
#include <libjs/runtime/temporal/PlainTimePrototype.h>
#include <libjs/runtime/temporal/PlainYearMonthConstructor.h>
#include <libjs/runtime/temporal/PlainYearMonthPrototype.h>
#include <libjs/runtime/temporal/Temporal.h>
#include <libjs/runtime/temporal/TimeZoneConstructor.h>
#include <libjs/runtime/temporal/TimeZonePrototype.h>
#include <libjs/runtime/temporal/ZonedDateTimeConstructor.h>
#include <libjs/runtime/temporal/ZonedDateTimePrototype.h>
#include <libjs/runtime/TypedArray.h>
#include <libjs/runtime/TypedArrayConstructor.h>
#include <libjs/runtime/TypedArrayPrototype.h>
#include <libjs/runtime/Value.h>
#include <libjs/runtime/WeakMapConstructor.h>
#include <libjs/runtime/WeakMapPrototype.h>
#include <libjs/runtime/WeakRefConstructor.h>
#include <libjs/runtime/WeakRefPrototype.h>
#include <libjs/runtime/WeakSetConstructor.h>
#include <libjs/runtime/WeakSetPrototype.h>

namespace JS {

GlobalObject::GlobalObject()
    : Object(GlobalObjectTag::Tag)
    , m_console(make<Console>(*this))
{
}

void GlobalObject::initialize_global_object()
{
    auto& vm = this->vm();

    ensure_shape_is_unique();

    m_empty_object_shape = heap().allocate_without_global_object<Shape>(*this);
    m_object_prototype = heap().allocate_without_global_object<ObjectPrototype>(*this);
    m_function_prototype = heap().allocate_without_global_object<FunctionPrototype>(*this);

    m_environment = heap().allocate<GlobalEnvironment>(*this, *this);

    m_new_object_shape = vm.heap().allocate_without_global_object<Shape>(*this);
    m_new_object_shape->set_prototype_without_transition(m_object_prototype);

    m_new_ordinary_function_prototype_object_shape = vm.heap().allocate_without_global_object<Shape>(*this);
    m_new_ordinary_function_prototype_object_shape->set_prototype_without_transition(m_object_prototype);
    m_new_ordinary_function_prototype_object_shape->add_property_without_transition(vm.names.constructor, Attribute::Writable | Attribute::Configurable);

    static_cast<FunctionPrototype*>(m_function_prototype)->initialize(*this);
    static_cast<ObjectPrototype*>(m_object_prototype)->initialize(*this);

    auto success = Object::internal_set_prototype_of(m_object_prototype);
    VERIFY(success);

    m_error_prototype = heap().allocate<ErrorPrototype>(*this, *this);

#define __JS_ENUMERATE(ClassName, snake_name) \
    if (!m_##snake_name##_prototype)          \
        m_##snake_name##_prototype = heap().allocate<ClassName##Prototype>(*this, *this);
    JS_ENUMERATE_ITERATOR_PROTOTYPES
#undef __JS_ENUMERATE

    m_generator_object_prototype = heap().allocate<GeneratorObjectPrototype>(*this, *this);
    m_generator_object_prototype->define_direct_property(vm.names.constructor, m_generator_function_constructor, Attribute::Configurable);

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName, ArrayType) \
    if (!m_##snake_name##_prototype)                                                     \
        m_##snake_name##_prototype = heap().allocate<PrototypeName>(*this, *this);
    JS_ENUMERATE_BUILTIN_TYPES
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName) \
    if (!m_intl_##snake_name##_prototype)                                     \
        m_intl_##snake_name##_prototype = heap().allocate<Intl::PrototypeName>(*this, *this);
    JS_ENUMERATE_INTL_OBJECTS
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName) \
    initialize_constructor(vm.names.ClassName, m_intl_##snake_name##_constructor, m_intl_##snake_name##_prototype);
    JS_ENUMERATE_INTL_OBJECTS
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName) \
    if (!m_temporal_##snake_name##_prototype)                                 \
        m_temporal_##snake_name##_prototype = heap().allocate<Temporal::PrototypeName>(*this, *this);
    JS_ENUMERATE_TEMPORAL_OBJECTS
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName) \
    initialize_constructor(vm.names.ClassName, m_temporal_##snake_name##_constructor, m_temporal_##snake_name##_prototype);
    JS_ENUMERATE_TEMPORAL_OBJECTS
#undef __JS_ENUMERATE

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.gc, gc, 0, attr);
    define_native_function(vm.names.isNaN, is_nan, 1, attr);
    define_native_function(vm.names.isFinite, is_finite, 1, attr);
    define_native_function(vm.names.parseFloat, parse_float, 1, attr);
    define_native_function(vm.names.parseInt, parse_int, 2, attr);
    define_native_function(vm.names.eval, eval, 1, attr);

    m_throw_type_error_function = NativeFunction::create(global_object(), {}, [](VM& vm, GlobalObject& global_object) {
        vm.throw_exception<TypeError>(global_object, ErrorType::RestrictedFunctionPropertiesAccess);
        return Value();
    });
    m_throw_type_error_function->define_direct_property(vm.names.length, Value(0), 0);
    m_throw_type_error_function->define_direct_property(vm.names.name, js_string(vm, ""), 0);
    m_throw_type_error_function->internal_prevent_extensions();

    m_function_prototype->define_direct_accessor(vm.names.caller, m_throw_type_error_function, m_throw_type_error_function, Attribute::Configurable);
    m_function_prototype->define_direct_accessor(vm.names.arguments, m_throw_type_error_function, m_throw_type_error_function, Attribute::Configurable);

    define_native_function(vm.names.encodeURI, encode_uri, 1, attr);
    define_native_function(vm.names.decodeURI, decode_uri, 1, attr);
    define_native_function(vm.names.encodeURIComponent, encode_uri_component, 1, attr);
    define_native_function(vm.names.decodeURIComponent, decode_uri_component, 1, attr);
    define_native_function(vm.names.escape, escape, 1, attr);
    define_native_function(vm.names.unescape, unescape, 1, attr);

    define_direct_property(vm.names.NaN, js_nan(), 0);
    define_direct_property(vm.names.Infinity, js_infinity(), 0);
    define_direct_property(vm.names.undefined, js_undefined(), 0);

    define_direct_property(vm.names.globalThis, this, attr);
    define_direct_property(vm.names.console, heap().allocate<ConsoleObject>(*this, *this), attr);
    define_direct_property(vm.names.Atomics, heap().allocate<AtomicsObject>(*this, *this), attr);
    define_direct_property(vm.names.Math, heap().allocate<MathObject>(*this, *this), attr);
    define_direct_property(vm.names.JSON, heap().allocate<JSONObject>(*this, *this), attr);
    define_direct_property(vm.names.Reflect, heap().allocate<ReflectObject>(*this, *this), attr);
    define_direct_property(vm.names.Intl, heap().allocate<Intl::Intl>(*this, *this), attr);
    define_direct_property(vm.names.Temporal, heap().allocate<Temporal::Temporal>(*this, *this), attr);

    initialize_constructor(vm.names.Error, m_error_constructor, m_error_prototype);

    add_constructor(vm.names.AggregateError, m_aggregate_error_constructor, m_aggregate_error_prototype);
    add_constructor(vm.names.Array, m_array_constructor, m_array_prototype);
    add_constructor(vm.names.ArrayBuffer, m_array_buffer_constructor, m_array_buffer_prototype);
    add_constructor(vm.names.BigInt, m_bigint_constructor, m_bigint_prototype);
    add_constructor(vm.names.Boolean, m_boolean_constructor, m_boolean_prototype);
    add_constructor(vm.names.DataView, m_data_view_constructor, m_data_view_prototype);
    add_constructor(vm.names.Date, m_date_constructor, m_date_prototype);
    add_constructor(vm.names.Error, m_error_constructor, m_error_prototype);
    add_constructor(vm.names.FinalizationRegistry, m_finalization_registry_constructor, m_finalization_registry_prototype);
    add_constructor(vm.names.Function, m_function_constructor, m_function_prototype);
    add_constructor(vm.names.Map, m_map_constructor, m_map_prototype);
    add_constructor(vm.names.Number, m_number_constructor, m_number_prototype);
    add_constructor(vm.names.Object, m_object_constructor, m_object_prototype);
    add_constructor(vm.names.Promise, m_promise_constructor, m_promise_prototype);
    add_constructor(vm.names.Proxy, m_proxy_constructor, nullptr);
    add_constructor(vm.names.RegExp, m_regexp_constructor, m_regexp_prototype);
    add_constructor(vm.names.Set, m_set_constructor, m_set_prototype);
    add_constructor(vm.names.String, m_string_constructor, m_string_prototype);
    add_constructor(vm.names.Symbol, m_symbol_constructor, m_symbol_prototype);
    add_constructor(vm.names.WeakMap, m_weak_map_constructor, m_weak_map_prototype);
    add_constructor(vm.names.WeakRef, m_weak_ref_constructor, m_weak_ref_prototype);
    add_constructor(vm.names.WeakSet, m_weak_set_constructor, m_weak_set_prototype);

    initialize_constructor(vm.names.TypedArray, m_typed_array_constructor, m_typed_array_prototype);

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName, ArrayType) \
    add_constructor(vm.names.ClassName, m_##snake_name##_constructor, m_##snake_name##_prototype);
    JS_ENUMERATE_NATIVE_ERRORS
    JS_ENUMERATE_TYPED_ARRAYS
#undef __JS_ENUMERATE

    m_generator_function_constructor = heap().allocate<GeneratorFunctionConstructor>(*this, *this);
    m_generator_function_prototype->define_direct_property(vm.names.constructor, m_generator_function_constructor, Attribute::Configurable);

    m_array_prototype_values_function = &m_array_prototype->get_without_side_effects(vm.names.values).as_function();
    m_eval_function = &get_without_side_effects(vm.names.eval).as_function();
    m_temporal_time_zone_prototype_get_offset_nanoseconds_for_function = &m_temporal_time_zone_prototype->get_without_side_effects(vm.names.getOffsetNanosecondsFor).as_function();
}

GlobalObject::~GlobalObject()
{
}

void GlobalObject::visit_edges(Visitor& visitor)
{
    Base::visit_edges(visitor);

    visitor.visit(m_empty_object_shape);
    visitor.visit(m_new_object_shape);
    visitor.visit(m_new_ordinary_function_prototype_object_shape);
    visitor.visit(m_proxy_constructor);
    visitor.visit(m_generator_object_prototype);
    visitor.visit(m_environment);
    visitor.visit(m_array_prototype_values_function);
    visitor.visit(m_eval_function);
    visitor.visit(m_temporal_time_zone_prototype_get_offset_nanoseconds_for_function);
    visitor.visit(m_throw_type_error_function);

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName, ArrayType) \
    visitor.visit(m_##snake_name##_constructor);                                         \
    visitor.visit(m_##snake_name##_prototype);
    JS_ENUMERATE_BUILTIN_TYPES
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName) \
    visitor.visit(m_intl_##snake_name##_constructor);                         \
    visitor.visit(m_intl_##snake_name##_prototype);
    JS_ENUMERATE_INTL_OBJECTS
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName) \
    visitor.visit(m_temporal_##snake_name##_constructor);                     \
    visitor.visit(m_temporal_##snake_name##_prototype);
    JS_ENUMERATE_TEMPORAL_OBJECTS
#undef __JS_ENUMERATE

#define __JS_ENUMERATE(ClassName, snake_name) \
    visitor.visit(m_##snake_name##_prototype);
    JS_ENUMERATE_ITERATOR_PROTOTYPES
#undef __JS_ENUMERATE
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::gc)
{
#ifdef __pranaos__
    dbgln("Forced garbage collection requested!");
#endif
    vm.heap().collect_garbage();
    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::is_nan)
{
    auto number = vm.argument(0).to_number(global_object);
    if (vm.exception())
        return {};
    return Value(number.is_nan());
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::is_finite)
{
    auto number = vm.argument(0).to_number(global_object);
    if (vm.exception())
        return {};
    return Value(number.is_finite_number());
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::parse_float)
{
    if (vm.argument(0).is_number())
        return vm.argument(0);
    auto input_string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto trimmed_string = input_string.trim_whitespace(TrimMode::Left);
    for (size_t length = trimmed_string.length(); length > 0; --length) {

        auto number = Value(js_string(vm, trimmed_string.substring(0, length))).to_number(global_object);
        if (!number.is_nan())
            return number;
    }
    return js_nan();
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::parse_int)
{
    auto input_string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};

    double sign = 1;
    auto s = input_string.trim_whitespace(TrimMode::Left);
    if (!s.is_empty() && s[0] == '-')
        sign = -1;
    if (!s.is_empty() && (s[0] == '+' || s[0] == '-'))
        s = s.substring(1, s.length() - 1);

    auto radix = vm.argument(1).to_i32(global_object);
    if (vm.exception())
        return {};

    bool strip_prefix = true;
    if (radix != 0) {
        if (radix < 2 || radix > 36)
            return js_nan();
        if (radix != 16)
            strip_prefix = false;
    } else {
        radix = 10;
    }

    if (strip_prefix) {
        if (s.length() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            s = s.substring(2, s.length() - 2);
            radix = 16;
        }
    }

    auto parse_digit = [&](u32 code_point, i32 radix) -> Optional<i32> {
        if (!is_ascii_alphanumeric(code_point) || radix <= 0)
            return {};
        auto digit = parse_ascii_base36_digit(code_point);
        if (digit >= (u32)radix)
            return {};
        return digit;
    };

    bool had_digits = false;
    double number = 0;
    for (auto code_point : Utf8View(s)) {
        auto digit = parse_digit(code_point, radix);
        if (!digit.has_value())
            break;
        had_digits = true;
        number *= radix;
        number += digit.value();
    }

    if (!had_digits)
        return js_nan();

    return Value(sign * number);
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::eval)
{
    return perform_eval(vm.argument(0), global_object, CallerMode::NonStrict, EvalMode::Indirect);
}

static String encode([[maybe_unused]] JS::GlobalObject& global_object, const String& string, StringView unescaped_set)
{
    StringBuilder encoded_builder;
    for (unsigned char code_unit : string) {
        if (unescaped_set.contains(code_unit)) {
            encoded_builder.append(code_unit);
            continue;
        }

        encoded_builder.appendff("%{:02X}", code_unit);
    }
    return encoded_builder.build();
}

static String decode(JS::GlobalObject& global_object, const String& string, StringView reserved_set)
{
    StringBuilder decoded_builder;
    auto expected_continuation_bytes = 0;
    for (size_t k = 0; k < string.length(); k++) {
        auto code_unit = string[k];
        if (code_unit != '%') {
            if (expected_continuation_bytes > 0) {
                global_object.vm().throw_exception<URIError>(global_object, ErrorType::URIMalformed);
                return {};
            }
            decoded_builder.append(code_unit);
            continue;
        }
        if (k + 2 >= string.length()) {
            global_object.vm().throw_exception<URIError>(global_object, ErrorType::URIMalformed);
            return {};
        }
        auto first_digit = decode_hex_digit(string[k + 1]);
        if (first_digit >= 16) {
            global_object.vm().throw_exception<URIError>(global_object, ErrorType::URIMalformed);
            return {};
        }
        auto second_digit = decode_hex_digit(string[k + 2]);
        if (second_digit >= 16) {
            global_object.vm().throw_exception<URIError>(global_object, ErrorType::URIMalformed);
            return {};
        }
        char decoded_code_unit = (first_digit << 4) | second_digit;
        k += 2;
        if (expected_continuation_bytes > 0) {
            decoded_builder.append(decoded_code_unit);
            expected_continuation_bytes--;
            continue;
        }
        if ((decoded_code_unit & 0x80) == 0) {
            if (reserved_set.contains(decoded_code_unit))
                decoded_builder.append(string.substring_view(k - 2, 3));
            else
                decoded_builder.append(decoded_code_unit);
            continue;
        }
        auto leading_ones = count_trailing_zeroes_32_safe(~decoded_code_unit) - 24;
        if (leading_ones == 1 || leading_ones > 4) {
            global_object.vm().throw_exception<URIError>(global_object, ErrorType::URIMalformed);
            return {};
        }
        decoded_builder.append(decoded_code_unit);
        expected_continuation_bytes = leading_ones - 1;
    }
    return decoded_builder.build();
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::encode_uri)
{
    auto uri_string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto encoded = encode(global_object, uri_string, ";/?:@&=+$,abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.!~*'()#"sv);
    if (vm.exception())
        return {};
    return js_string(vm, move(encoded));
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::decode_uri)
{
    auto uri_string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto decoded = decode(global_object, uri_string, ";/?:@&=+$,#"sv);
    if (vm.exception())
        return {};
    return js_string(vm, move(decoded));
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::encode_uri_component)
{
    auto uri_string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto encoded = encode(global_object, uri_string, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.!~*'()"sv);
    if (vm.exception())
        return {};
    return js_string(vm, move(encoded));
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::decode_uri_component)
{
    auto uri_string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto decoded = decode(global_object, uri_string, ""sv);
    if (vm.exception())
        return {};
    return js_string(vm, move(decoded));
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::escape)
{
    auto string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    StringBuilder escaped;
    for (auto code_point : Utf8View(string)) {
        if (code_point < 256) {
            if ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@*_+-./"sv.contains(code_point))
                escaped.append(code_point);
            else
                escaped.appendff("%{:02X}", code_point);
            continue;
        }
        escaped.appendff("%u{:04X}", code_point);
    }
    return js_string(vm, escaped.build());
}

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::unescape)
{
    auto string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    ssize_t length = string.length();
    StringBuilder unescaped(length);
    for (auto k = 0; k < length; ++k) {
        u32 code_point = string[k];
        if (code_point == '%') {
            if (k <= length - 6 && string[k + 1] == 'u' && is_ascii_hex_digit(string[k + 2]) && is_ascii_hex_digit(string[k + 3]) && is_ascii_hex_digit(string[k + 4]) && is_ascii_hex_digit(string[k + 5])) {
                code_point = (parse_ascii_hex_digit(string[k + 2]) << 12) | (parse_ascii_hex_digit(string[k + 3]) << 8) | (parse_ascii_hex_digit(string[k + 4]) << 4) | parse_ascii_hex_digit(string[k + 5]);
                k += 5;
            } else if (k <= length - 3 && is_ascii_hex_digit(string[k + 1]) && is_ascii_hex_digit(string[k + 2])) {
                code_point = (parse_ascii_hex_digit(string[k + 1]) << 4) | parse_ascii_hex_digit(string[k + 2]);
                k += 2;
            }
        }
        unescaped.append_code_point(code_point);
    }
    return js_string(vm, unescaped.build());
}

}
