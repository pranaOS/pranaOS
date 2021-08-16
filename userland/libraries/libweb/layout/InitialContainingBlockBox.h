/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Document.h>
#include <libweb/layout/BlockBox.h>

namespace Web::Layout {

class InitialContainingBlockBox final : public BlockBox {
public:
    explicit InitialContainingBlockBox(DOM::Document&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~InitialContainingBlockBox() override;

    const DOM::Document& dom_node() const { return static_cast<const DOM::Document&>(*Node::dom_node()); }

    void paint_all_phases(PaintContext&);

    virtual HitTestResult hit_test(const Gfx::IntPoint&, HitTestType) const override;

    const LayoutRange& selection() const { return m_selection; }
    void set_selection(const LayoutRange&);
    void set_selection_end(const LayoutPosition&);

    void build_stacking_context_tree();

    void recompute_selection_states();

private:
    LayoutRange m_selection;
};

}
