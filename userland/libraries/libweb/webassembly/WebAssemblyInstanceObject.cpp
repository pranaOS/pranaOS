/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/ScopeGuard.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/ArrayBuffer.h>
#include <libjs/runtime/BigInt.h>
#include <libjs/runtime/TypedArray.h>
#include <libwasm/abstractmachine/Interpreter.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/webassembly/WebAssemblyInstanceObject.h>
#include <libweb/webassembly/WebAssemblyMemoryPrototype.h>
#include <libweb/webassembly/WebAssemblyObject.h>

namespace Web::Bindings {

WebAssemblyInstanceObject::WebAssemblyInstanceObject(JS::GlobalObject& global_object, size_t index)
    : Object(static_cast<Web::Bindings::WindowObject&>(global_object).ensure_web_prototype<WebAssemblyInstancePrototype>("WebAssemblyInstancePrototype"))
    , m_index(index)
{
}

void WebAssemblyInstanceObject::initialize(JS::GlobalObject& global_object)
{
    Object::initialize(global_object);

    VERIFY(!m_exports_object);
    m_exports_object = create(global_object, nullptr);
    auto& instance = this->instance();
    auto& cache = this->cache();
    for (auto& export_ : instance.exports()) {
        export_.value().visit(
            [&](const Wasm::FunctionAddress& address) {
                auto object = cache.function_instances.get(address);
                if (!object.has_value()) {
                    object = create_native_function(address, export_.name(), global_object);
                    cache.function_instances.set(address, *object);
                }
                m_exports_object->define_direct_property(export_.name(), *object, JS::default_attributes);
            },
            [&](const Wasm::MemoryAddress& address) {
                auto object = cache.memory_instances.get(address);
                if (!object.has_value()) {
                    object = heap().allocate<Web::Bindings::WebAssemblyMemoryObject>(global_object, global_object, address);
                    cache.memory_instances.set(address, *object);
                }
                m_exports_object->define_direct_property(export_.name(), *object, JS::default_attributes);
            },
            [&](const auto&) {
                
            });
    }

    m_exports_object->set_integrity_level(IntegrityLevel::Frozen);
}

void WebAssemblyInstanceObject::visit_edges(Visitor& visitor)
{
    Object::visit_edges(visitor);
    visitor.visit(m_exports_object);
}

}
