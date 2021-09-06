/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

namespace WinServer {

// Common for widget and panel item.
enum MenuItemAnswer {
    Empty = 0x0,
    Bad = 0x1, // Bad mark
    InvalidateMe = 0x2, // Asks to invalidate menu item
    PopupShow = 0x4, // Asks to show popup. MenuBar will call popup_rect()
    PopupClose = 0x8, // Asks to close popup.
};

}; // namespace WinServer