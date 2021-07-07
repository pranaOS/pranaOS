/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

enum MouseButtonState
{
    MOUSE_BUTTON_RELEASED,
    MOUSE_BUTTON_PRESSED,
};

struct MousePacket
{
    int offx;
    int offy;
    int scroll;
};