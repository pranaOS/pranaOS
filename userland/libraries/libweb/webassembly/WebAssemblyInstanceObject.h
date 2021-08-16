/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/VM.h>
#include <libwasm/abstractmachine/AbstractMachine.h>
#include <libweb/Forward.h>
#include <libweb/webassembly/WebAssemblyObject.h>

namespace Web::Bindings {

class WebAssemblyInstanceObject final : public JS::Object {
    JS_OBJECT(WebAssemblyInstanceObject, Object);

public:
    explicit WebAssemblyInstanceObject(JS::GlobalObject&, size_t index);
    virtual void initialize(JS::GlobalObject&) override;
    virtual ~WebAssemblyInstanceObject() override = default;

    size_t index() const { return m_index; }
    Wasm::ModuleInstance& instance() const { return WebAssemblyObject::s_instantiated_modules.at(m_index); }
    auto& cache() { return WebAssemblyObject::s_module_caches.at(m_index); }

    void visit_edges(Visitor&) override;

    friend class WebAssemblyInstancePrototype;

private:
    size_t m_index { 0 };
    Object* m_exports_object { nullptr };
};

}
