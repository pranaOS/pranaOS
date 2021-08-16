/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/Box.h>

namespace Web::Layout {

class InlineNode : public NodeWithStyleAndBoxModelMetrics {
public:
    InlineNode(DOM::Document&, DOM::Element&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~InlineNode() override;

    virtual void paint_fragment(PaintContext&, const LineBoxFragment&, PaintPhase) const override;

    virtual void split_into_lines(InlineFormattingContext&, LayoutMode) override;
};

}
