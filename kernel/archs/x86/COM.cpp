/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "archs/x86/COM.h"
#include "archs/x86_32/x86_32.h"
#include "system/interrupts/Interupts.h"

bool com_can_read(COMPort port)
{
    return (in8(port + 5) & 0b0000001);
}

bool com_can_write(COMPort port)
{
    return (in8(port + 5) & 0b0100000);
}

void com_wait_write(COMPort port)
{
    while (!com_can_write(port))
    { 
    }
}

void com_wait_read(COMPort port)
{
    while (!com_can_read(port))
    { 
    }
}

void com_putc(COMPort port, char c)
{
    com_wait_write(port);
    out8(port, c);
}