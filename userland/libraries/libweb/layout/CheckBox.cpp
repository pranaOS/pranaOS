/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libgui/Event.h>
#include <libgfx/Font.h>
#include <libgfx/Painter.h>
#include <libgfx/StylePainter.h>
#include <libweb/layout/CheckBox.h>
#include <libweb/layout/Label.h>
#include <libweb/page/BrowsingContext.h>

namespace Web::Layout {

CheckBox::CheckBox(DOM::Document& document, HTML::HTMLInputElement& element, NonnullRefPtr<CSS::StyleProperties> style)
    : LabelableNode(document, element, move(style))
{
    set_has_intrinsic_width(true);
    set_has_intrinsic_height(true);
    set_intrinsic_width(13);
    set_intrinsic_height(13);
}

CheckBox::~CheckBox()
{
}

void CheckBox::paint(PaintContext& context, PaintPhase phase)
{
    if (!is_visible())
        return;

    LabelableNode::paint(context, phase);

    if (phase == PaintPhase::Foreground) {
        Gfx::StylePainter::paint_check_box(context.painter(), enclosing_int_rect(absolute_rect()), context.palette(), dom_node().enabled(), dom_node().checked(), m_being_pressed);
    }
}

void CheckBox::handle_mousedown(Badge<EventHandler>, const Gfx::IntPoint&, unsigned button, unsigned)
{
    if (button != GUI::MouseButton::Left || !dom_node().enabled())
        return;

    m_being_pressed = true;
    set_needs_display();

    m_tracking_mouse = true;
    browsing_context().event_handler().set_mouse_event_tracking_layout_node(this);
}

void CheckBox::handle_mouseup(Badge<EventHandler>, const Gfx::IntPoint& position, unsigned button, unsigned)
{
    if (!m_tracking_mouse || button != GUI::MouseButton::Left || !dom_node().enabled())
        return;

    NonnullRefPtr protect = *this;

    bool is_inside_node_or_label = enclosing_int_rect(absolute_rect()).contains(position);
    if (!is_inside_node_or_label)
        is_inside_node_or_label = Label::is_inside_associated_label(*this, position);

    if (is_inside_node_or_label)
        dom_node().set_checked(!dom_node().checked());

    m_being_pressed = false;
    m_tracking_mouse = false;
    browsing_context().event_handler().set_mouse_event_tracking_layout_node(nullptr);
}

void CheckBox::handle_mousemove(Badge<EventHandler>, const Gfx::IntPoint& position, unsigned, unsigned)
{
    if (!m_tracking_mouse || !dom_node().enabled())
        return;

    bool is_inside_node_or_label = enclosing_int_rect(absolute_rect()).contains(position);
    if (!is_inside_node_or_label)
        is_inside_node_or_label = Label::is_inside_associated_label(*this, position);

    if (m_being_pressed == is_inside_node_or_label)
        return;

    m_being_pressed = is_inside_node_or_label;
    set_needs_display();
}

void CheckBox::handle_associated_label_mousedown(Badge<Label>)
{
    m_being_pressed = true;
    set_needs_display();
}

void CheckBox::handle_associated_label_mouseup(Badge<Label>)
{
    NonnullRefPtr protect = *this;

    dom_node().set_checked(!dom_node().checked());
    m_being_pressed = false;
}

void CheckBox::handle_associated_label_mousemove(Badge<Label>, bool is_inside_node_or_label)
{
    if (m_being_pressed == is_inside_node_or_label)
        return;

    m_being_pressed = is_inside_node_or_label;
    set_needs_display();
}

}
