/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>
#include <libweb/dom/Event.h>
#include <libweb/Forward.h>

namespace Web::DOM {

class EventDispatcher {
public:
    static bool dispatch(NonnullRefPtr<EventTarget>, NonnullRefPtr<Event>, bool legacy_target_override = false);

private:
    static void invoke(Event::PathEntry&, Event&, Event::Phase);
    static bool inner_invoke(Event&, Vector<EventTarget::EventListenerRegistration>&, Event::Phase, bool);
};

}
