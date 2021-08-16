/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/bindings/PerformanceWrapper.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Event.h>
#include <libweb/dom/EventDispatcher.h>
#include <libweb/dom/Window.h>
#include <libweb/highresolutiontime/Performance.h>

namespace Web::HighResolutionTime {

Performance::Performance(DOM::Window& window)
    : DOM::EventTarget(static_cast<Bindings::ScriptExecutionContext&>(window.document()))
    , m_window(window)
    , m_timing(make<NavigationTiming::PerformanceTiming>(window))
{
    m_timer.start();
}

Performance::~Performance()
{
}

double Performance::time_origin() const
{
    auto origin = m_timer.origin_time();
    return (origin.tv_sec * 1000.0) + (origin.tv_usec / 1000.0);
}

void Performance::ref_event_target()
{
    m_window.ref();
}

void Performance::unref_event_target()
{
    m_window.unref();
}

bool Performance::dispatch_event(NonnullRefPtr<DOM::Event> event)
{
    return DOM::EventDispatcher::dispatch(*this, event);
}

JS::Object* Performance::create_wrapper(JS::GlobalObject& global_object)
{
    return Bindings::wrap(global_object, *this);
}

}
