/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullOwnPtrVector.h>
#include <base/Vector.h>
#include <libweb/layout/LineBoxFragment.h>

namespace Web::Layout {

class LineBox {
public:
    LineBox() { }

    float width() const { return m_width; }

    void add_fragment(Node& layout_node, int start, int length, float width, float height, LineBoxFragment::Type = LineBoxFragment::Type::Normal);

    const NonnullOwnPtrVector<LineBoxFragment>& fragments() const { return m_fragments; }
    NonnullOwnPtrVector<LineBoxFragment>& fragments() { return m_fragments; }

    void trim_trailing_whitespace();

    bool is_empty_or_ends_in_whitespace() const;

private:
    friend class BlockBox;
    friend class InlineFormattingContext;
    NonnullOwnPtrVector<LineBoxFragment> m_fragments;
    float m_width { 0 };
};

}
