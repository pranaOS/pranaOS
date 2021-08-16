/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/svg/SVGGraphicsElement.h>

namespace Web::SVG {

class SVGGeometryElement : public SVGGraphicsElement {
public:
    using WrapperType = Bindings::SVGGeometryElementWrapper;

protected:
    SVGGeometryElement(DOM::Document& document, QualifiedName qualified_name);
};

}
