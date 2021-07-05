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