/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/webassembly/WebAssemblyInstanceConstructor.h>
#include <libweb/webassembly/WebAssemblyInstanceObject.h>
#include <libweb/webassembly/WebAssemblyInstanceObjectPrototype.h>
#include <libweb/webassembly/WebAssemblyModuleObject.h>
#include <libweb/webassembly/WebAssemblyObject.h>

namespace Web::Bindings {

WebAssemblyInstanceConstructor::WebAssemblyInstanceConstructor(JS::GlobalObject& global_object)
    : NativeFunction(*global_object.function_prototype())
{
}

WebAssemblyInstanceConstructor::~WebAssemblyInstanceConstructor()
{
}

JS::Value WebAssemblyInstanceConstructor::call()
{
    vm().throw_exception<JS::TypeError>(global_object(), JS::ErrorType::ConstructorWithoutNew, "WebAssembly.Instance");
    return {};
}

JS::Value WebAssemblyInstanceConstructor::construct(FunctionObject&)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto module_argument = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};

    if (!is<WebAssemblyModuleObject>(module_argument)) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::NotA, "WebAssembly.Module");
        return {};
    }

    auto& module_object = static_cast<WebAssemblyModuleObject&>(*module_argument);
    auto result = WebAssemblyObject::instantiate_module(module_object.module(), vm, global_object);
    if (result.is_error()) {
        vm.throw_exception(global_object, result.release_error());
        return {};
    }
    return heap().allocate<WebAssemblyInstanceObject>(global_object, global_object, result.value());
}

void WebAssemblyInstanceConstructor::initialize(JS::GlobalObject& global_object)
{
    auto& vm = this->vm();
    auto& window = static_cast<WindowObject&>(global_object);

    NativeFunction::initialize(global_object);
    define_direct_property(vm.names.prototype, &window.ensure_web_prototype<WebAssemblyInstancePrototype>("WebAssemblyInstancePrototype"), 0);
    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
}

}
