/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#include "archs/x86_32/x86_32.h"

enum CMOSTimeSelector
{
    T_SECOND = 0x0,
    T_MINUTE = 0x2,
    T_HOUR = 0x4,
    T_DAY = 0x7,
    T_MONTH = 0x8,
    T_YEAR = 0x9,
};


#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

#define from_bcd(val) (((val) / 16) * 10 + ((val)&0xf))

#define CMOS_WAIT while (is_cmos_update())


bool is_cmos_update()
{
    out8(CMOS_ADDRESS, 0x0A);
    return (in8(CMOS_DATA) & 0x80);
}

char get_realtime_reg(CMOSTimeSelector reg)
{
    out8(CMOS_ADDRESS, reg);
    return in8(CMOS_DATA);
}
