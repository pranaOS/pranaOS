/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Array.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/ProxyConstructor.h>
#include <libjs/runtime/ProxyObject.h>

namespace JS {

static ProxyObject* proxy_create(GlobalObject& global_object, Value target, Value handler)
{
    auto& vm = global_object.vm();
    if (!target.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyConstructorBadType, "target", target.to_string_without_side_effects());
        return {};
    }
    if (!handler.is_object()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ProxyConstructorBadType, "handler", handler.to_string_without_side_effects());
        return {};
    }
    return ProxyObject::create(global_object, target.as_object(), handler.as_object());
}

ProxyConstructor::ProxyConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Proxy.as_string(), *global_object.function_prototype())
{
}

void ProxyConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.revocable, revocable, 2, attr);

    define_direct_property(vm.names.length, Value(2), Attribute::Configurable);
}

ProxyConstructor::~ProxyConstructor()
{
}

Value ProxyConstructor::call()
{
    auto& vm = this->vm();
    vm.throw_exception<TypeError>(global_object(), ErrorType::ConstructorWithoutNew, vm.names.Proxy);
    return {};
}

Value ProxyConstructor::construct(FunctionObject&)
{
    auto& vm = this->vm();
    return proxy_create(global_object(), vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(ProxyConstructor::revocable)
{
    auto* proxy = proxy_create(global_object, vm.argument(0), vm.argument(1));
    if (vm.exception())
        return {};
 
    auto* revoker = NativeFunction::create(global_object, "", [proxy_handle = make_handle(proxy)](auto&, auto&) -> Value {
        auto& proxy = const_cast<ProxyObject&>(*proxy_handle.cell());
        if (proxy.is_revoked())
            return js_undefined();
    
        proxy.revoke();
        return js_undefined();
    });
    revoker->define_direct_property(vm.names.length, Value(0), Attribute::Configurable);
    revoker->define_direct_property(vm.names.name, js_string(vm, String::empty()), Attribute::Configurable);

    auto* result = Object::create(global_object, global_object.object_prototype());
    result->create_data_property_or_throw(vm.names.proxy, proxy);
    result->create_data_property_or_throw(vm.names.revoke, revoker);
    return result;
}

}
