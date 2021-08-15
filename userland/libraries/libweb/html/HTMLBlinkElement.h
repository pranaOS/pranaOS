/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libcore/Forward.h>
#include <libweb/HTML/HTMLElement.h>

namespace Web::HTML {

class HTMLBlinkElement final : public HTMLElement {
public:
    HTMLBlinkElement(DOM::Document&, QualifiedName);
    virtual ~HTMLBlinkElement() override;

private:
    void blink();

    NonnullRefPtr<Core::Timer> m_timer;
};

}
