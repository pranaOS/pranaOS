/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLIFrameElement.h>
#include <libweb/layout/ReplacedBox.h>

namespace Web::Layout {

class FrameBox final : public ReplacedBox {
public:
    FrameBox(DOM::Document&, DOM::Element&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~FrameBox() override;

    virtual void paint(PaintContext&, PaintPhase) override;
    virtual void prepare_for_replaced_layout() override;

    const HTML::HTMLIFrameElement& dom_node() const { return verify_cast<HTML::HTMLIFrameElement>(ReplacedBox::dom_node()); }
    HTML::HTMLIFrameElement& dom_node() { return verify_cast<HTML::HTMLIFrameElement>(ReplacedBox::dom_node()); }

private:
    virtual void did_set_rect() override;
};

}
