/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libgfx/Size.h>

namespace Web::Layout {

struct PixelBox {
    float top { 0 };
    float right { 0 };
    float bottom { 0 };
    float left { 0 };
};

struct BoxModelMetrics {
public:
    PixelBox margin;
    PixelBox padding;
    PixelBox border;
    PixelBox offset;

    PixelBox margin_box() const;
    PixelBox padding_box() const;
    PixelBox border_box() const;
};

}
