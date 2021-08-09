/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/ringbuffer.h>
#include <drivers/driver_manager.h>
#include <drivers/x86/display.h>
#include <drivers/x86/mouse.h>
#include <fs/devfs/devfs.h>
#include <libkernel/kassert.h>
#include <libkernel/log.h>
#include <libkernel/types.h>
#include <platform/x86/idt.h>
#include <platform/x86/port.h>

static ringbuffer_t mouse_buffer;

void mouse_run();

static bool _mouse_can_read(dentry_t* dentry, uint32_t start)
{
    return ringbuffer_space_to_read(&mouse_buffer) >= 1;
}

static inline void _mouse_wait_in()
{
    while ((port_8bit_in(0x64) & 1) == 0) { }
}

static inline void _mouse_wait_out()
{
    while ((port_8bit_in(0x64) & 2) == 1) { }
}

static inline void _mouse_wait_then_write(uint16_t port, uint8_t data)
{
    _mouse_wait_out();
    port_8bit_out(port, data);
}

// #ifndef MOUSE_DRIVER_DEBUG
//     log("%x", packet.button_states);
// #endif


void _mouse_run()
{
    mouse_buffer = ringbuffer_create_std();

    _mouse_wait_then_write(0x64, 0xa8);
    _mouse_can_read();
}

bool mouse_install()
{
    driver_install(_mouse_driver_info(), "mouse86");
    return true;
}