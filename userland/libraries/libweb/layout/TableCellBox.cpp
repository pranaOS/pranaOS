/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Element.h>
#include <libweb/Layout/TableCellBox.h>
#include <libweb/Layout/TableRowBox.h>

namespace Web::Layout {

TableCellBox::TableCellBox(DOM::Document& document, DOM::Element* element, NonnullRefPtr<CSS::StyleProperties> style)
    : Layout::BlockBox(document, element, move(style))
{
}

TableCellBox::TableCellBox(DOM::Document& document, DOM::Element* element, CSS::ComputedValues computed_values)
    : Layout::BlockBox(document, element, move(computed_values))
{
}

TableCellBox::~TableCellBox()
{
}

size_t TableCellBox::colspan() const
{
    if (!dom_node())
        return 1;
    return verify_cast<DOM::Element>(*dom_node()).attribute(HTML::AttributeNames::colspan).to_uint().value_or(1);
}

float TableCellBox::width_of_logical_containing_block() const
{
    if (auto* row = first_ancestor_of_type<TableRowBox>())
        return row->width();
    return 0;
}

}
