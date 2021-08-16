/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/bindings/EventTargetWrapperFactory.h>
#include <libweb/dom/EventDispatcher.h>
#include <libweb/dom/EventTarget.h>

namespace Web::XHR {

class XMLHttpRequestEventTarget
    : public DOM::EventTarget
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::XMLHttpRequestEventTargetWrapper;

    virtual ~XMLHttpRequestEventTarget() override {};

protected:
    explicit XMLHttpRequestEventTarget(Bindings::ScriptExecutionContext& script_execution_context)
        : DOM::EventTarget(script_execution_context)
    {
    }

private:
    virtual JS::Object* create_wrapper(JS::GlobalObject& global_object) override
    {
        return wrap(global_object, *this);
    }
};

}
