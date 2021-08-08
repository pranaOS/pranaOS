/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/aarch32/pl050.h>
#include <drivers/generic/keyboard.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkern/log.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>
#include <platform/aarch32/interrupts.h>

#define DEBUG_PL050

static zone_t mapped_zone;
static volatile pl050_registers_t* registers = (pl050_registers_t*)PL050_KEYBOARD_BASE;

static inline int _pl050_map_itself()
{
    mapped_zone = zoner_new_zone(sizeof(pl050_registers_t));
    vmm_map_page(mapped_zone.start, PL050_KEYBOARD_BASE, PAGE_READABLE | PAGE_WRITABLE | PAGE_EXECUTABLE);
    registers = (pl050_registers_t*)mapped_zone.ptr;
    return 0;

}

static void _pl050_keyboard_int_handler()
{
    uint32_t data = registers->data;
    generic_emit_key_set1(data);
}

static inline void _keyboard_send_cmd(uint8_t cmd)
{
    registers->data = cmd;
    while ((registers->stat) & (1 << 5)) { }
    int tmp = regisers->data;
    ASSERT(tmp == 0xfa);
}

void pl050_keyboard_install()
{
    if (_pl050_map_itself()) {
#ifndef DEBUG_PL050
        log_error("PL050 KBD: Can't map itself!!!");
#endif 
        return;
    }
    driver_install(_pl050_keyboard_info(), "pl050kb");
}