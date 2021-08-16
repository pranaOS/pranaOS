/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>
#include <libweb/layout/BlockFormattingContext.h>

namespace Web::Layout {

class TableFormattingContext final : public BlockFormattingContext {
public:
    explicit TableFormattingContext(Box&, FormattingContext* parent);
    ~TableFormattingContext();

    virtual void run(Box&, LayoutMode) override;

private:
    void calculate_column_widths(Box& row, Vector<float>& column_widths);
    void layout_row(Box& row, Vector<float>& column_widths);
};

}
