/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>
#include <base/NonnullOwnPtr.h>
#include <base/WeakPtr.h>
#include <kernel/api/KeyCode.h>
#include <libgui/Forward.h>
#include <libgfx/Forward.h>
#include <libweb/Forward.h>
#include <libweb/page/EditEventHandler.h>

namespace Web {

class BrowsingContext;

class EventHandler {
public:
    explicit EventHandler(Badge<BrowsingContext>, BrowsingContext&);
    ~EventHandler();

    bool handle_mouseup(const Gfx::IntPoint&, unsigned button, unsigned modifiers);
    bool handle_mousedown(const Gfx::IntPoint&, unsigned button, unsigned modifiers);
    bool handle_mousemove(const Gfx::IntPoint&, unsigned buttons, unsigned modifiers);
    bool handle_mousewheel(const Gfx::IntPoint&, unsigned buttons, unsigned modifiers, int wheel_delta);

    bool handle_keydown(KeyCode, unsigned modifiers, u32 code_point);

    void set_mouse_event_tracking_layout_node(Layout::Node*);

    void set_edit_event_handler(NonnullOwnPtr<EditEventHandler> value) { m_edit_event_handler = move(value); }

private:
    bool focus_next_element();
    bool focus_previous_element();

    Layout::InitialContainingBlockBox* layout_root();
    const Layout::InitialContainingBlockBox* layout_root() const;

    BrowsingContext& m_frame;

    bool m_in_mouse_selection { false };

    WeakPtr<Layout::Node> m_mouse_event_tracking_layout_node;

    NonnullOwnPtr<EditEventHandler> m_edit_event_handler;
};

}
