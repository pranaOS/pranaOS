/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Math.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/NumberConstructor.h>
#include <libjs/runtime/NumberObject.h>

#ifdef __clang__
#    define EPSILON_VALUE AK::exp2(-52.)
#    define MAX_SAFE_INTEGER_VALUE AK::exp2(53.) - 1
#    define MIN_SAFE_INTEGER_VALUE -(AK::exp2(53.) - 1)
#else
constexpr const double EPSILON_VALUE { __builtin_exp2(-52) };
constexpr const double MAX_SAFE_INTEGER_VALUE { __builtin_exp2(53) - 1 };
constexpr const double MIN_SAFE_INTEGER_VALUE { -(__builtin_exp2(53) - 1) };
#endif

namespace JS {

NumberConstructor::NumberConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Number.as_string(), *global_object.function_prototype())
{
}

void NumberConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.number_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.isFinite, is_finite, 1, attr);
    define_native_function(vm.names.isInteger, is_integer, 1, attr);
    define_native_function(vm.names.isNaN, is_nan, 1, attr);
    define_native_function(vm.names.isSafeInteger, is_safe_integer, 1, attr);
    define_direct_property(vm.names.parseInt, global_object.get(vm.names.parseInt), attr);
    define_direct_property(vm.names.parseFloat, global_object.get(vm.names.parseFloat), attr);
    define_direct_property(vm.names.EPSILON, Value(EPSILON_VALUE), 0);
    define_direct_property(vm.names.MAX_VALUE, Value(NumericLimits<double>::max()), 0);
    define_direct_property(vm.names.MIN_VALUE, Value(NumericLimits<double>::min()), 0);
    define_direct_property(vm.names.MAX_SAFE_INTEGER, Value(MAX_SAFE_INTEGER_VALUE), 0);
    define_direct_property(vm.names.MIN_SAFE_INTEGER, Value(MIN_SAFE_INTEGER_VALUE), 0);
    define_direct_property(vm.names.NEGATIVE_INFINITY, js_negative_infinity(), 0);
    define_direct_property(vm.names.POSITIVE_INFINITY, js_infinity(), 0);
    define_direct_property(vm.names.NaN, js_nan(), 0);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

NumberConstructor::~NumberConstructor()
{
}

static Value get_value_from_constructor_argument(GlobalObject& global_object)
{
    auto& vm = global_object.vm();

    Value number;
    if (vm.argument_count() > 0) {
        auto primitive = vm.argument(0).to_numeric(global_object);
        if (vm.exception())
            return {};
        if (primitive.is_bigint()) {

            auto& big_integer = primitive.as_bigint().big_integer();
            number = Value(static_cast<double>(big_integer.unsigned_value().to_u64()) * (big_integer.is_negative() ? -1.0 : 1.0));
        } else {
            number = primitive;
        }
    } else {
        number = Value(0);
    }
    return number;
}

Value NumberConstructor::call()
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto number = get_value_from_constructor_argument(global_object);
    if (vm.exception())
        return {};
    return number;
}

Value NumberConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto number = get_value_from_constructor_argument(global_object);
    if (vm.exception())
        return {};
    return ordinary_create_from_constructor<NumberObject>(global_object, new_target, &GlobalObject::number_prototype, number.as_double());
}

JS_DEFINE_NATIVE_FUNCTION(NumberConstructor::is_finite)
{
    return Value(vm.argument(0).is_finite_number());
}

JS_DEFINE_NATIVE_FUNCTION(NumberConstructor::is_integer)
{
    return Value(vm.argument(0).is_integral_number());
}

JS_DEFINE_NATIVE_FUNCTION(NumberConstructor::is_nan)
{
    return Value(vm.argument(0).is_nan());
}

JS_DEFINE_NATIVE_FUNCTION(NumberConstructor::is_safe_integer)
{
    if (!vm.argument(0).is_number())
        return Value(false);
    if (!vm.argument(0).is_integral_number())
        return Value(false);
    auto value = vm.argument(0).as_double();
    return Value(value >= MIN_SAFE_INTEGER_VALUE && value <= MAX_SAFE_INTEGER_VALUE);
}

}
