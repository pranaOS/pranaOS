/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/Box.h>

namespace Web::Layout {

class TableRowBox final : public Box {
public:
    TableRowBox(DOM::Document&, DOM::Element*, NonnullRefPtr<CSS::StyleProperties>);
    TableRowBox(DOM::Document&, DOM::Element*, CSS::ComputedValues);
    virtual ~TableRowBox() override;

    static CSS::Display static_display() { return CSS::Display::TableRow; }
};

}
