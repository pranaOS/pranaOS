/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>
#include <libwasm/abstractmachine/AbstractMachine.h>
#include <libweb/Forward.h>
#include <libweb/webassembly/WebAssemblyInstanceObjectPrototype.h>
#include <libweb/webassembly/WebAssemblyObject.h>

namespace Web::Bindings {

class WebAssemblyModuleObject final : public JS::Object {
    JS_OBJECT(WebAssemblyModuleObject, Object);

public:
    explicit WebAssemblyModuleObject(JS::GlobalObject&, size_t index);
    virtual ~WebAssemblyModuleObject() override = default;

    size_t index() const { return m_index; }
    const Wasm::Module& module() const { return WebAssemblyObject::s_compiled_modules.at(m_index).module; }

private:
    size_t m_index { 0 };
};

}
