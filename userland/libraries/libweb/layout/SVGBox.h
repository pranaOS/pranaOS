/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/ReplacedBox.h>
#include <libweb/svg/SVGElement.h>
#include <libweb/svg/SVGGraphicsElement.h>

namespace Web::Layout {

class SVGBox : public ReplacedBox {
public:
    SVGBox(DOM::Document&, SVG::SVGElement&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~SVGBox() override = default;

    virtual void before_children_paint(PaintContext& context, PaintPhase phase) override;
    virtual void after_children_paint(PaintContext& context, PaintPhase phase) override;
};

}
