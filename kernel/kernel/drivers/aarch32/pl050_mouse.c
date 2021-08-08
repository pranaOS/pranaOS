/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/ringbuffer.h>
#include <drivers/aarch32/pl050.h>
#include <drivers/generic/mouse.h>
#include <fs/devfs/devfs.h>
#include <fs/vfs.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkern/log.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>
#include <platform/aarch32/interrupts.h>
#include <tasking/tasking.h>

static ringbuffer_t mouse_buffer;
static zone mapped_zone;
static volatile pl050_registers_t* registers = (pl050_registers_t*)PL050_MOUSE_BASE;

static inline int _pl050_map_itself()
{
    mapped_zone = zoner_new_zone(sizeof(pl050_registers_t));
    vmm_map_page(mapped_zone.start, PL050_MOUSE_BASE, PAGE_READABLE | PAGE_WRITABLE | PAGE_EXECUTABLE);
    registers = (pl050_registers_t*)mapped_zone.ptr;
    return 0;
}

static bool _mouse_can_read(dentry_t* dentry, uint32_t start)
{
    return ringbuffer_space_to_read(&mouse_buffer) >= 1;
}

void pl050_mouse_install()
{
    if (_pl050_map_itself()) {
#ifndef DEBUG_PL050
        log_error("PL050 MOUSE: Can't map itself!!");
#endif
        return;
    }
    driver_install(_pl050_mouse_driver_info(), "pl050ms");

}

static inline void _mouse_send_cmd(uint8_t cmd)
{
    registers->data = cmd;
    while ((registers->stat) & (1 << 5)) { }
    int tmp = registers->data;
    ASSERT(tmp == 0xfa);
}

#define KMIIR_RXINTR (1 << 0)
#define RXFULL (1 << 4)

void pl050_mouse_init(device_t* dev)
{
#ifdef DEBUG_PL050
    log("PL050 MOUSE: Turning on");
#endif
    registers->cr = 0x4 | 0x10;
    _mouse_send_cmd(0xF6);
    _mouse_send_cmd(0xF4);
    irq_register_handler(PL050_MOUSE_IRQ_LINE, 0, 0, _pl050_mouse_int_handler, BOOT_CPU_MASK);
    mouse_buffer = ringbuffer_create_std();
}