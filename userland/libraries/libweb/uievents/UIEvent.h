/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Event.h>

namespace Web::UIEvents {

class UIEvent : public DOM::Event {
public:
    using WrapperType = Bindings::UIEventWrapper;

    virtual ~UIEvent() override { }

protected:
    explicit UIEvent(const FlyString& event_name)
        : Event(event_name)
    {
    }
};

}
