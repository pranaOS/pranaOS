/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Element.h>
#include <libweb/layout/TableBox.h>

namespace Web::Layout {

TableBox::TableBox(DOM::Document& document, DOM::Element* element, NonnullRefPtr<CSS::StyleProperties> style)
    : Layout::BlockBox(document, element, move(style))
{
}

TableBox::TableBox(DOM::Document& document, DOM::Element* element, CSS::ComputedValues computed_values)
    : Layout::BlockBox(document, element, move(computed_values))
{
}

TableBox::~TableBox()
{
}

}
