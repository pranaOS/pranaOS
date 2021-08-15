/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/DocumentFragment.h>

namespace Web::DOM {

class ShadowRoot final : public DocumentFragment {
public:
    ShadowRoot(Document&, Element&);

    bool closed() const { return m_closed; }

    bool delegates_focus() const { return m_delegates_focus; }
    void set_delegates_focus(bool delegates_focus) { m_delegates_focus = delegates_focus; }

    bool available_to_element_internals() const { return m_available_to_element_internals; }
    void set_available_to_element_internals(bool available_to_element_internals) { m_available_to_element_internals = available_to_element_internals; }

    virtual EventTarget* get_parent(const Event&) override;

    String mode() const { return m_closed ? "closed" : "open"; }

private:

    virtual FlyString node_name() const override { return "#shadow-root"; }
    virtual RefPtr<Layout::Node> create_layout_node() override;

    bool m_closed { false };
    bool m_delegates_focus { false };
    bool m_available_to_element_internals { false };
};

}
