/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Vector.h>
#include <libweb/layout/Node.h>

namespace Web::Layout {

class StackingContext {
public:
    StackingContext(Box&, StackingContext* parent);

    StackingContext* parent() { return m_parent; }
    const StackingContext* parent() const { return m_parent; }

    enum class StackingContextPaintPhase {
        BackgroundAndBorders,
        Floats,
        Foreground,
        FocusAndOverlay,
    };

    void paint_descendants(PaintContext&, Node&, StackingContextPaintPhase);
    void paint(PaintContext&);
    HitTestResult hit_test(const Gfx::IntPoint&, HitTestType) const;

    void dump(int indent = 0) const;

private:
    Box& m_box;
    StackingContext* const m_parent { nullptr };
    Vector<StackingContext*> m_children;

    void paint_internal(PaintContext&);
};

}
