/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Element.h>
#include <libweb/layout/BlockBox.h>

namespace Web::Layout {

class ListItemMarkerBox;

class ListItemBox final : public BlockBox {
public:
    ListItemBox(DOM::Document&, DOM::Element&, NonnullRefPtr<CSS::StyleProperties>);
    virtual ~ListItemBox() override;

    void layout_marker();

private:
    RefPtr<ListItemMarkerBox> m_marker;
};

}
