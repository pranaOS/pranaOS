/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/layout/BlockBox.h>
#include <libweb/layout/BreakNode.h>
#include <libweb/layout/InlineFormattingContext.h>

namespace Web::Layout {

BreakNode::BreakNode(DOM::Document& document, HTML::HTMLBRElement& element)
    : Layout::NodeWithStyleAndBoxModelMetrics(document, &element, CSS::StyleProperties::create())
{
    set_inline(true);
}

BreakNode::~BreakNode()
{
}

void BreakNode::split_into_lines(InlineFormattingContext& context, LayoutMode)
{
    auto& line_box = context.containing_block().add_line_box();
    line_box.add_fragment(*this, 0, 0, 0, context.containing_block().line_height());
}

}
