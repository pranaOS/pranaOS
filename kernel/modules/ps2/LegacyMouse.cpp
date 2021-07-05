/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "ps2/LegacyMouse.h"

void LegacyMouse::wait(int type)
{
    int time_out = 100000;
    if (type == 0)
    {
        while (time_out--)
        {
            if ((in8(PS2_STATUS) & 1) == 1)
            {
                return;
            }
        }
        return;
    }
    else
    {
        while (time_out--)
        {
            if ((in8(PS2_STATUS) & 2) == 0)
            {
                return;
            }
        }
        return;
    }
}

void LegacyMouse::write_register(uint8_t value)
{
    wait(1);
    out8(0x64, 0xD4);
    wait(1);
    out8(0x60, value);
}

uint8_t LegacyMouse::read_register()
{
    wait(0);
    return in8(0x60);
}

void LegacyMouse::handle_finished_packet(uint8_t packet0, uint8_t packet1, uint8_t packet2, uint8_t packet3)
{
    int offx = packet1;

    if (offx && (packet0 & (1 << 4)))
    {
        offx -= 0x100;
    }

    int offy = packet2;

    if (offy && (packet0 & (1 << 5)))
    {
        offy -= 0x100;
    }

    int scroll = 0;

    if (!_quirk_no_mouse_whell)
    {
        scroll = (int8_t)packet3;
    }

    // decode the new mouse packet
    MousePacket event;

    event.offx = offx;
    event.offy = -offy;
    event.scroll = scroll;
    event.middle = (MouseButtonState)((packet0 >> 2) & 1);
    event.right = (MouseButtonState)((packet0 >> 1) & 1);
    event.left = (MouseButtonState)((packet0)&1);

    if (_events.write((const char *)&event, sizeof(MousePacket)) != sizeof(MousePacket))
    {
        Kernel::logln("Mouse buffer overflow!");
    }
}
