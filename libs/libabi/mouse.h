/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

enum MouseButtonState
{
    MOUSE_BUtTON_RELEASE,
};

struct MousePacket
{
    int offx;
    int offy;
    int scroll;

    MouseButtonState left;
    MouseButtonState right;
    MouseButtonState middle;
};
