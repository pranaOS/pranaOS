/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "WebAssemblyModulePrototype.h"
#include <libweb/webassembly/WebAssemblyModuleObject.h>

namespace Web::Bindings {

WebAssemblyModuleObject::WebAssemblyModuleObject(JS::GlobalObject& global_object, size_t index)
    : Object(static_cast<WindowObject&>(global_object).ensure_web_prototype<WebAssemblyModulePrototype>("WebAssemblyModulePrototype"))
    , m_index(index)
{
}

}
