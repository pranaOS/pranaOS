/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/TypedArray.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/webassembly/WebAssemblyModuleConstructor.h>
#include <libweb/webassembly/WebAssemblyModuleObject.h>
#include <libweb/webassembly/WebAssemblyModulePrototype.h>
#include <libweb/webassembly/WebAssemblyObject.h>

namespace Web::Bindings {

WebAssemblyModuleConstructor::WebAssemblyModuleConstructor(JS::GlobalObject& global_object)
    : NativeFunction(*global_object.function_prototype())
{
}

WebAssemblyModuleConstructor::~WebAssemblyModuleConstructor()
{
}

JS::Value WebAssemblyModuleConstructor::call()
{
    vm().throw_exception<JS::TypeError>(global_object(), JS::ErrorType::ConstructorWithoutNew, "WebAssembly.Module");
    return {};
}

JS::Value WebAssemblyModuleConstructor::construct(FunctionObject&)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto buffer_object = vm.argument(0).to_object(global_object);
    if (vm.exception())
        return {};

    auto result = parse_module(global_object, buffer_object);
    if (result.is_error()) {
        vm.throw_exception(global_object, result.error());
        return {};
    }

    return heap().allocate<WebAssemblyModuleObject>(global_object, global_object, result.release_value());
}

void WebAssemblyModuleConstructor::initialize(JS::GlobalObject& global_object)
{
    auto& vm = this->vm();
    auto& window = static_cast<WindowObject&>(global_object);

    NativeFunction::initialize(global_object);
    define_direct_property(vm.names.prototype, &window.ensure_web_prototype<WebAssemblyModulePrototype>("WebAssemblyModulePrototype"), 0);
    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
}

}
