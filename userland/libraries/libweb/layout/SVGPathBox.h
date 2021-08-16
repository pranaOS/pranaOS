/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/SVGGraphicsBox.h>
#include <libweb/svg/SVGPathElement.h>

namespace Web::Layout {

class SVGPathBox final : public SVGGraphicsBox {
public:
    SVGPathBox(DOM::Document&, SVG::SVGPathElement&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~SVGPathBox() override = default;

    SVG::SVGPathElement& dom_node() { return verify_cast<SVG::SVGPathElement>(SVGGraphicsBox::dom_node()); }

    virtual void prepare_for_replaced_layout() override;
    virtual void paint(PaintContext& context, PaintPhase phase) override;
};

}
