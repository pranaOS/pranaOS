/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libweb/bindings/EventListenerWrapper.h>
#include <libweb/dom/EventListener.h>

namespace Web {
namespace Bindings {

EventListenerWrapper::EventListenerWrapper(JS::GlobalObject& global_object, DOM::EventListener& impl)
    : Wrapper(*global_object.object_prototype())
    , m_impl(impl)
{
}

EventListenerWrapper::~EventListenerWrapper()
{
}

}
}
