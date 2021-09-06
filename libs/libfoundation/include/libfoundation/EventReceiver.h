/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libfoundation/Event.h>
#include <memory>

namespace LFoundation {

class EventReceiver {
public:
    EventReceiver() = default;
    ~EventReceiver() = default;
    virtual void receive_event(std::unique_ptr<Event> event) { }

private:
};

} // namespace LFoundation