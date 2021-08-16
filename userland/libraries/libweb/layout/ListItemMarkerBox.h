/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/Box.h>

namespace Web::Layout {

class ListItemMarkerBox final : public Box {
public:
    explicit ListItemMarkerBox(DOM::Document&, CSS::ListStyleType, size_t index);
    virtual ~ListItemMarkerBox() override;

    virtual void paint(PaintContext&, PaintPhase) override;

private:
    CSS::ListStyleType m_list_style_type { CSS::ListStyleType::None };
    size_t m_index;

    String m_text {};
};

}
