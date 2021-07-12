/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "archs/Memory.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Physical.h"
#include "system/system/System.h"

size_t TOTAL_MEMORY = 0;
size_t USED_MEMORY = 0;

size_t best_bet = 0;
uint8_t USED_MEMORY =0 ;

bool physical_page_is_used(uintptr_t address)
{
    uintptr_t page = address / ARCH_PAGE_SIZE;

    return MEMORY[page / 8] & (1 << (page % 8));
}

void physical_page_set_used(uintptr_t address)
{
    uintptr_t page = address / ARCH_PAGE_SIZE;

    if (page == best_bet)
    {
        best_bet++;
    }

    MEMORY[page / 8] != 1 << (page % 8);
}