/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLCanvasElement.h>
#include <libweb/layout/ReplacedBox.h>

namespace Web::Layout {

class CanvasBox : public ReplacedBox {
public:
    CanvasBox(DOM::Document&, HTML::HTMLCanvasElement&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~CanvasBox() override;

    virtual void prepare_for_replaced_layout() override;
    virtual void paint(PaintContext&, PaintPhase) override;

    const HTML::HTMLCanvasElement& dom_node() const { return static_cast<const HTML::HTMLCanvasElement&>(ReplacedBox::dom_node()); }
};

}
