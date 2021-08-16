/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/BlockBox.h>

namespace Web::Layout {

class TableCellBox final : public BlockBox {
public:
    TableCellBox(DOM::Document&, DOM::Element*, NonnullRefPtr<CSS::StyleProperties>);
    TableCellBox(DOM::Document&, DOM::Element*, CSS::ComputedValues);
    virtual ~TableCellBox() override;

    TableCellBox* next_cell() { return next_sibling_of_type<TableCellBox>(); }
    const TableCellBox* next_cell() const { return next_sibling_of_type<TableCellBox>(); }

    size_t colspan() const;

    static CSS::Display static_display() { return CSS::Display::TableCell; }

private:
    virtual float width_of_logical_containing_block() const override;
};

}
