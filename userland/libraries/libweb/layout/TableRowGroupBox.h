/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/BlockBox.h>

namespace Web::Layout {

class TableRowGroupBox final : public BlockBox {
public:
    TableRowGroupBox(DOM::Document&, DOM::Element&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~TableRowGroupBox() override;

    size_t column_count() const;
};

}
