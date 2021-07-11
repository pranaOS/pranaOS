/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/Streams.h"
#include <libutils/Prelude.h>


namespace Arch::x86_64
{

#define GDT_ENTRY_COUNT 5

#define GDT_SEGMENT (0b00010000)
#define GDT_PRESENT (0b10000000)
#define GDT_USER (0b01100000)
#define GDT_EXECUTABLE (0b00001000)
#define GDT_READWRITE (0b00000010)

#define GDT_LONG_MODE_GRANULARITY 0b0010

#define GDT_FLAGS 0b1100

struct PACKED TSS64
{
    uint32_t reserved;

    uint64_t rsp[3];

    uint64_t reserved0;

    uint64_t ist[7];

    uint32_t reserved1;
    uint32_t reserved2;
    uint16_t reserved3;

    uint16_t iopb_offset;
};

struct PACKED GDTDescriptor64
{
    uint16_t size;
    uint64_t offset;
};

}