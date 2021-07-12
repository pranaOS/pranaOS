/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include <extramodules/stivale/stivale2.h>
#include "system/Streams.h"
#include "system/handover/Handover.h"

bool is_stivale2(uint32_t magic)
{
    return magic == 0x73747632;
}

void stivale2_parse_header(Handover *handover, void *header_ptr)
{
    auto info = reinterpret_cast<stivale2_struct *>(header_ptr);
    strlcpy(handover->bootloader, info->bootloader_brand, HANDOVER_BOOTLOADER_NAME_SIZE);

    auto tag = reinterpret_cast<stivale2_tag *>(info->tags);
}