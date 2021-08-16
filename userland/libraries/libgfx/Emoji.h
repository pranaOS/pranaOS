/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Types.h>

namespace Gfx {

class Bitmap;

class Emoji {
public:
    static const Gfx::Bitmap* emoji_for_code_point(u32 code_point);
};

}
