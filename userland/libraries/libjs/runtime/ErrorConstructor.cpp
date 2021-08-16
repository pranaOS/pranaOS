/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/ErrorConstructor.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

ErrorConstructor::ErrorConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Error.as_string(), *global_object.function_prototype())
{
}

void ErrorConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    // 20.5.2.1 Error.prototype, https://tc39.es/ecma262/#sec-error.prototype
    define_direct_property(vm.names.prototype, global_object.error_prototype(), 0);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

// 20.5.1.1 Error ( message ), https://tc39.es/ecma262/#sec-error-message
Value ErrorConstructor::call()
{
    return construct(*this);
}

// 20.5.1.1 Error ( message ), https://tc39.es/ecma262/#sec-error-message
Value ErrorConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto* error = ordinary_create_from_constructor<Error>(global_object, new_target, &GlobalObject::error_prototype);
    if (vm.exception())
        return {};

    if (!vm.argument(0).is_undefined()) {
        auto message = vm.argument(0).to_string(global_object);
        if (vm.exception())
            return {};
        error->create_non_enumerable_data_property_or_throw(vm.names.message, js_string(vm, message));
    }

    error->install_error_cause(vm.argument(1));
    if (vm.exception())
        return {};

    return error;
}

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName, ArrayType)                   \
    ConstructorName::ConstructorName(GlobalObject& global_object)                                          \
        : NativeFunction(*static_cast<Object*>(global_object.error_constructor()))                         \
    {                                                                                                      \
    }                                                                                                      \
                                                                                                           \
    void ConstructorName::initialize(GlobalObject& global_object)                                          \
    {                                                                                                      \
        auto& vm = this->vm();                                                                             \
        NativeFunction::initialize(global_object);                                                         \
                                                                                                           \
        /* 20.5.6.2.1 NativeError.prototype,                                                               \
           https://tc39.es/ecma262/#sec-nativeerror.prototype */                                           \
        define_direct_property(vm.names.prototype, global_object.snake_name##_prototype(), 0);             \
                                                                                                           \
        define_direct_property(vm.names.length, Value(1), Attribute::Configurable);                        \
    }                                                                                                      \
                                                                                                           \
    ConstructorName::~ConstructorName() { }                                                                \
                                                                                                           \
    /* 20.5.6.1.1 NativeError ( message ), https://tc39.es/ecma262/#sec-nativeerror */                     \
    Value ConstructorName::call()                                                                          \
    {                                                                                                      \
        return construct(*this);                                                                           \
    }                                                                                                      \
                                                                                                           \
    /* 20.5.6.1.1 NativeError ( message ), https://tc39.es/ecma262/#sec-nativeerror */                     \
    Value ConstructorName::construct(FunctionObject& new_target)                                           \
    {                                                                                                      \
        auto& vm = this->vm();                                                                             \
        auto& global_object = this->global_object();                                                       \
                                                                                                           \
        auto* error = ordinary_create_from_constructor<ClassName>(                                         \
            global_object, new_target, &GlobalObject::snake_name##_prototype);                             \
        if (vm.exception())                                                                                \
            return {};                                                                                     \
                                                                                                           \
        if (!vm.argument(0).is_undefined()) {                                                              \
            auto message = vm.argument(0).to_string(global_object);                                        \
            if (vm.exception())                                                                            \
                return {};                                                                                 \
            error->create_non_enumerable_data_property_or_throw(vm.names.message, js_string(vm, message)); \
        }                                                                                                  \
                                                                                                           \
        error->install_error_cause(vm.argument(1));                                                        \
        if (vm.exception())                                                                                \
            return {};                                                                                     \
                                                                                                           \
        return error;                                                                                      \
    }

JS_ENUMERATE_NATIVE_ERRORS
#undef __JS_ENUMERATE

}
