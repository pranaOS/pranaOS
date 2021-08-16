/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/svg/SVGElement.h>

namespace Web::SVG {

SVGElement::SVGElement(DOM::Document& document, QualifiedName qualified_name)
    : Element(document, move(qualified_name))
{
}

}
