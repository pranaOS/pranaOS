/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/devices/PCSpeaker.h>
#include <kernel/IO.h>
#include <kernel/time/PIT.h>


void PCSpeaker::tone_on()
{
    IO::out8(0x61, IO::in8(0x61) & ~3);

}