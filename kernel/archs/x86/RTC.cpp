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
