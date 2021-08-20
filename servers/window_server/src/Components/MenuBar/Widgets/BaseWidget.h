/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "../MenuItemAnswer.h"
#include <libg/Context.h>

namespace WinServer {

class BaseWidget {
public:
    BaseWidget() = default;
    virtual ~BaseWidget() = default;

    size_t height() { return 20; }
    virtual size_t width() { return 0; }
    virtual void draw(LG::Context& ctx) { }
    virtual MenuItemAnswer mouse_down(int x, int y) { return MenuItemAnswer::Empty; }
    virtual MenuItemAnswer mouse_up() { return MenuItemAnswer::Empty; }
    virtual void popup_rect(LG::Rect& r) { }
};

} // namespace WinServer