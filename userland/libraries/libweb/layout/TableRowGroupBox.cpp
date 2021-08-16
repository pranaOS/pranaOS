/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Element.h>
#include <libweb/layout/TableCellBox.h>
#include <libweb/layout/TableRowBox.h>
#include <libweb/layout/TableRowGroupBox.h>

namespace Web::Layout {

TableRowGroupBox::TableRowGroupBox(DOM::Document& document, DOM::Element& element, NonnullRefPtr<CSS::StyleProperties> style)
    : Layout::BlockBox(document, &element, move(style))
{
}

TableRowGroupBox::~TableRowGroupBox()
{
}

size_t TableRowGroupBox::column_count() const
{
    size_t table_column_count = 0;
    for_each_child_of_type<TableRowBox>([&](auto& row) {
        size_t row_column_count = 0;
        row.template for_each_child_of_type<TableCellBox>([&](auto& cell) {
            row_column_count += cell.colspan();
        });
        table_column_count = max(table_column_count, row_column_count);
    });
    return table_column_count;
}

}
