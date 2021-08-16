/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include "WebAssemblyModuleConstructor.h"
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/VM.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/Forward.h>

namespace Web::Bindings {

class WebAssemblyModulePrototype final : public JS::Object {
    JS_OBJECT(WebAssemblyModulePrototype, JS::Object);

public:
    explicit WebAssemblyModulePrototype(JS::GlobalObject& global_object)
        : JS::Object(global_object)
    {
    }
};

}
