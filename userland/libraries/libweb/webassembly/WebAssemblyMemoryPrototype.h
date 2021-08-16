/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include "WebAssemblyMemoryConstructor.h"
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/VM.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/Forward.h>

namespace Web::Bindings {

class WebAssemblyMemoryPrototype final : public JS::Object {
    JS_OBJECT(WebAssemblyMemoryPrototype, JS::Object);

public:
    explicit WebAssemblyMemoryPrototype(JS::GlobalObject& global_object)
        : JS::Object(global_object)
    {
    }

    virtual void initialize(JS::GlobalObject&) override;

private:
    JS_DECLARE_NATIVE_FUNCTION(grow);
    JS_DECLARE_NATIVE_FUNCTION(buffer_getter);
};

}
