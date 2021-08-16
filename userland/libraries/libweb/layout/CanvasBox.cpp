/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libgfx/Painter.h>
#include <libweb/layout/CanvasBox.h>

namespace Web::Layout {

CanvasBox::CanvasBox(DOM::Document& document, HTML::HTMLCanvasElement& element, NonnullRefPtr<CSS::StyleProperties> style)
    : ReplacedBox(document, element, move(style))
{
}

CanvasBox::~CanvasBox()
{
}

void CanvasBox::prepare_for_replaced_layout()
{
    set_has_intrinsic_width(true);
    set_has_intrinsic_height(true);
    set_intrinsic_width(dom_node().width());
    set_intrinsic_height(dom_node().height());
}

void CanvasBox::paint(PaintContext& context, PaintPhase phase)
{
    if (!is_visible())
        return;

    ReplacedBox::paint(context, phase);

    if (phase == PaintPhase::Foreground) {

        if (!context.viewport_rect().intersects(enclosing_int_rect(absolute_rect())))
            return;

        if (dom_node().bitmap())
            context.painter().draw_scaled_bitmap(enclosing_int_rect(absolute_rect()), *dom_node().bitmap(), dom_node().bitmap()->rect());
    }
}

}
