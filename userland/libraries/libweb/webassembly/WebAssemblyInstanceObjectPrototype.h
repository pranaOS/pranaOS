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
#include <libweb/Forward.h>

namespace Web::Bindings {

class WebAssemblyInstancePrototype final : public JS::Object {
    JS_OBJECT(WebAssemblyInstancePrototype, Object);

public:
    explicit WebAssemblyInstancePrototype(JS::GlobalObject& global_object)
        : Object(global_object)
    {
    }

    virtual void initialize(JS::GlobalObject&) override;

private:
    JS_DECLARE_NATIVE_FUNCTION(exports_getter);
    static JS::Handle<WebAssemblyInstancePrototype> s_instance;
};

}
