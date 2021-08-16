/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Optional.h>
#include <libgfx/Color.h>

namespace Gfx {

struct TextAttributes {
    Color color;
    Optional<Color> background_color;
    bool underline { false };
    bool bold { false };
};

}
