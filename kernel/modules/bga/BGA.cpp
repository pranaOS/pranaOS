/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <libmath/MinMax.h>
#include "bga/BGA.h"
#include "system/graphics/EarlyConsole.h"
#include "system/graphics/Graphics.h"
#include "system/handover/Handover.h"
#include "system/interrupts/Interupts.h"


void BGA::write_register(uint16_t address, uint16_t data)
{
    out16(BGA_ADDRESS, address);
    out16(BGA_DATA, data);
}

uint16_t BGA::read_register(uint16_t address)
{
    out16(BGA_ADDRESS, address);
    return in16(BGA_DATA);
}