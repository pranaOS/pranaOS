/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Event.h>

namespace Web::HTML {

class SubmitEvent final : public DOM::Event {
public:
    using WrapperType = Bindings::SubmitEventWrapper;

    static NonnullRefPtr<SubmitEvent> create(const FlyString& event_name, RefPtr<HTMLElement> submitter);

    virtual ~SubmitEvent() override;

    RefPtr<HTMLElement> submitter() const;

private:
    SubmitEvent(const FlyString& event_name, RefPtr<HTMLElement> submitter);

    RefPtr<HTMLElement> m_submitter;
};

}
