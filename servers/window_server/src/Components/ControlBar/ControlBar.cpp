/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ControlBar.h"
#include "../../Compositor.h"
#include <libg/ImageLoaders/PNGLoader.h>

namespace WinServer {

ControlBar* s_WinServer_ControlBar_the = nullptr;

ControlBar::ControlBar()
    : m_bounds(0, Screen::the().bounds().height() - height(), Screen::the().bounds().width(), height())
{
    s_WinServer_ControlBar_the = this;
    LG::PNG::PNGLoader loader;
    m_menu_icon = loader.load_from_file("/res/system/mobile/control.png");
    int x = width() / 2 - m_menu_icon.width() / 2;
    int y = Screen::the().bounds().height() - height() / 2 - m_menu_icon.height() / 2;
    m_button_bounds = LG::Rect(x, y, m_menu_icon.width(), m_menu_icon.height());
}

} // namespace WinServer