/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/String.h>
#include <libjs/runtime/BigInt.h>
#include <libjs/runtime/BigIntConstructor.h>
#include <libjs/runtime/BigIntObject.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/VM.h>

namespace JS {

BigIntConstructor::BigIntConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.BigInt.as_string(), *global_object.function_prototype())
{
}

void BigIntConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.bigint_prototype(), 0);


    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

BigIntConstructor::~BigIntConstructor()
{
}

Value BigIntConstructor::call()
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto value = vm.argument(0);

    auto primitive = value.to_primitive(global_object, Value::PreferredType::Number);
    if (vm.exception())
        return {};

    if (primitive.is_number())
        return number_to_bigint(global_object, primitive);

    return value.to_bigint(global_object);
}

Value BigIntConstructor::construct(FunctionObject&)
{
    vm().throw_exception<TypeError>(global_object(), ErrorType::NotAConstructor, "BigInt");
    return {};
}

JS_DEFINE_NATIVE_FUNCTION(BigIntConstructor::as_int_n)
{
    TODO();
}

JS_DEFINE_NATIVE_FUNCTION(BigIntConstructor::as_uint_n)
{
    TODO();
}

}
