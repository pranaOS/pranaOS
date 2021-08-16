/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/StdLibExtras.h>
#include <libcore/ElapsedTimer.h>
#include <libweb/bindings/Wrappable.h>
#include <libweb/dom/EventTarget.h>
#include <libweb/navigationtiming/PerformanceTiming.h>

namespace Web::HighResolutionTime {

class Performance final
    : public DOM::EventTarget
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::PerformanceWrapper;
    using AllowOwnPtr = TrueType;

    explicit Performance(DOM::Window&);
    ~Performance();

    double now() const { return m_timer.elapsed(); }
    double time_origin() const;

    RefPtr<NavigationTiming::PerformanceTiming> timing() { return *m_timing; }

    virtual void ref_event_target() override;
    virtual void unref_event_target() override;

    virtual bool dispatch_event(NonnullRefPtr<DOM::Event>) override;
    virtual JS::Object* create_wrapper(JS::GlobalObject&) override;

private:
    DOM::Window& m_window;
    Core::ElapsedTimer m_timer;

    OwnPtr<NavigationTiming::PerformanceTiming> m_timing;
};

}
