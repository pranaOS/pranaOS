/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Event.h>

namespace Web::XHR {


class ProgressEvent : public DOM::Event {
public:
    using WrapperType = Bindings::ProgressEventWrapper;

    static NonnullRefPtr<ProgressEvent> create(const FlyString& event_name, u32 transmitted, u32 length)
    {
        return adopt_ref(*new ProgressEvent(event_name, transmitted, length));
    }

    virtual ~ProgressEvent() override { }

    bool length_computable() const { return m_length_computable; }
    u32 loaded() const { return m_loaded; }
    u32 total() const { return m_total; }

protected:
    ProgressEvent(const FlyString& event_name, u32 transmitted, u32 length)
        : Event(event_name)
        , m_length_computable(length != 0)
        , m_loaded(transmitted)
        , m_total(length)
    {
    }

    bool m_length_computable { false };
    u32 m_loaded { 0 };
    u32 m_total { 0 };
};

}
