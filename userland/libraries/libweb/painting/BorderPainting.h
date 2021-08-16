/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libgfx/Forward.h>
#include <libweb/css/ComputedValues.h>

namespace Web::Painting {

enum class BorderEdge {
    Top,
    Right,
    Bottom,
    Left,
};
void paint_border(PaintContext&, BorderEdge, const Gfx::FloatRect&, const CSS::ComputedValues&);

}
