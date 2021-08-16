/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/SVGBox.h>
#include <libweb/svg/SVGElement.h>
#include <libweb/svg/SVGGraphicsElement.h>

namespace Web::Layout {

class SVGGraphicsBox : public SVGBox {
public:
    SVGGraphicsBox(DOM::Document&, SVG::SVGGraphicsElement&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~SVGGraphicsBox() override = default;

    virtual void before_children_paint(PaintContext& context, PaintPhase phase) override;
};

}
