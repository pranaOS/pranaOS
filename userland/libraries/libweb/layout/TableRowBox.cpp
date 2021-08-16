/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Element.h>
#include <libweb/layout/TableRowBox.h>

namespace Web::Layout {

TableRowBox::TableRowBox(DOM::Document& document, DOM::Element* element, NonnullRefPtr<CSS::StyleProperties> style)
    : Box(document, element, move(style))
{
}

TableRowBox::TableRowBox(DOM::Document& document, DOM::Element* element, CSS::ComputedValues computed_values)
    : Box(document, element, move(computed_values))
{
}

TableRowBox::~TableRowBox()
{
}

}
