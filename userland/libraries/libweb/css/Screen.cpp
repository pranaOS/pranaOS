/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libgfx/Rect.h>
#include <libweb/css/Screen.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Window.h>
#include <libweb/page/Page.h>

namespace Web::CSS {

Screen::Screen(DOM::Window& window)
    : m_window(window)
{
}

Gfx::IntRect Screen::screen_rect() const
{
    return m_window.document().page()->screen_rect();
}

}
