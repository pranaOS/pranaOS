/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/aarch32/uart.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>

volatile uint32_t* output = (uint32_t*)COM1;
static zone_t mapped_zone;

static inline int _uart_map_itself()
{
    mapped_zone = zoner_new_zone(VMM_PAGE_SIZE);
    vmm_map_page(mapped_zone.start, COM1, PAGE_READABLE | PAGE_WRITABLE | PAGE_EXECUTABLE);
    output = (uint32_t*)mapped_zone.ptr;
    return 0;
}

void uart_setup()
{
}


int uart_write(int port, uint8_t data)
{
    *output = data;
    return 0;
}

int uart_read(int port, uint8_t* data)
{
    return 0;
}

int uart_overwrite(int port, uint8_t* data)
{
    *output = uint8_t(data);
    return data;
}