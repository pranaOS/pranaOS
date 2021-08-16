/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/SVGGraphicsBox.h>
#include <libweb/svg/SVGSVGElement.h>

namespace Web::Layout {

class SVGSVGBox final : public SVGGraphicsBox {
public:
    SVGSVGBox(DOM::Document&, SVG::SVGSVGElement&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~SVGSVGBox() override = default;

    SVG::SVGSVGElement& dom_node() { return verify_cast<SVG::SVGSVGElement>(SVGGraphicsBox::dom_node()); }

    virtual void prepare_for_replaced_layout() override;

    virtual void before_children_paint(PaintContext& context, PaintPhase phase) override;
    virtual void after_children_paint(PaintContext& context, PaintPhase phase) override;

    virtual bool can_have_children() const override { return true; }
};

}
