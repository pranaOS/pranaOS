/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <kernel/CMOS.h>
#include <kernel/IO.h>

namespace CMOS {

u8 read(u8 index)
{
    IO::out8(0x70, index);
    return IO::in8(0x71);
}

void write(u8 index, u8 data)
{
    IO::out8(0x70, index);
    IO::out8(0x71, data);
}

}
