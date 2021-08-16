/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libgfx/Bitmap.h>
#include <libweb/svg/SVGGraphicsElement.h>

namespace Web::SVG {

class SVGSVGElement final : public SVGGraphicsElement {
public:
    using WrapperType = Bindings::SVGSVGElementWrapper;

    SVGSVGElement(DOM::Document&, QualifiedName);

    virtual RefPtr<Layout::Node> create_layout_node() override;

    unsigned width() const;
    unsigned height() const;

    virtual bool requires_svg_container() const override { return false; }
    virtual bool is_svg_container() const override { return true; }

private:
    RefPtr<Gfx::Bitmap> m_bitmap;
};

}
