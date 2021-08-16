/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Element.h>

namespace Web::SVG {

class SVGElement : public DOM::Element {
public:
    using WrapperType = Bindings::SVGElementWrapper;

    virtual bool requires_svg_container() const override { return true; }

protected:
    SVGElement(DOM::Document&, QualifiedName);
};

}
