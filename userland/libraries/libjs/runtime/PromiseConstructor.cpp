/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/Interpreter.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Promise.h>
#include <libjs/runtime/PromiseConstructor.h>
#include <libjs/runtime/PromiseReaction.h>

namespace JS {

PromiseConstructor::PromiseConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Promise.as_string(), *global_object.function_prototype())
{
}

void PromiseConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.promise_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;

    define_native_function(vm.names.reject, reject, 1, attr);
    define_native_function(vm.names.resolve, resolve, 1, attr);

    define_native_accessor(*vm.well_known_symbol_species(), symbol_species_getter, {}, Attribute::Configurable);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

Value PromiseConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.Promise);
    return {};
}

Value PromiseConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto executor = vm.argument(0);
    if (!executor.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::PromiseExecutorNotAFunction);
        return {};
    }

    auto* promise = ordinary_create_from_constructor<Promise>(global_object, new_target, &GlobalObject::promise_prototype);
    if (vm.exception())
        return {};

    auto [resolve_function, reject_function] = promise->create_resolving_functions();

    (void)vm.call(executor.as_function(), js_undefined(), &resolve_function, &reject_function);
    if (auto* exception = vm.exception()) {
        vm.clear_exception();
        vm.stop_unwind();
        (void)vm.call(reject_function, js_undefined(), exception->value());
    }
    return promise;
}

JS_DEFINE_NATIVE_FUNCTION(PromiseConstructor::all)
{
    TODO();
}

JS_DEFINE_NATIVE_FUNCTION(PromiseConstructor::all_settled)
{
    TODO();
}

JS_DEFINE_NATIVE_FUNCTION(PromiseConstructor::any)
{
    TODO();
}

JS_DEFINE_NATIVE_FUNCTION(PromiseConstructor::race)
{
    TODO();
}

JS_DEFINE_NATIVE_FUNCTION(PromiseConstructor::reject)
{
    auto* constructor = vm.this_value(global_object).to_object(global_object);
    if (!constructor)
        return {};
    auto promise_capability = new_promise_capability(global_object, constructor);
    if (vm.exception())
        return {};
    auto reason = vm.argument(0);
    [[maybe_unused]] auto result = vm.call(*promise_capability.reject, js_undefined(), reason);
    return promise_capability.promise;
}

JS_DEFINE_NATIVE_FUNCTION(PromiseConstructor::resolve)
{
    auto* constructor = vm.this_value(global_object).to_object(global_object);
    if (!constructor)
        return {};
    auto value = vm.argument(0);
    return promise_resolve(global_object, *constructor, value);
}

JS_DEFINE_NATIVE_GETTER(PromiseConstructor::symbol_species_getter)
{
    return vm.this_value(global_object);
}

}
