/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/ringbuffer.h>
#include <drivers/x86/keyboard.h>
#include <io/tty/tty.h>
#include <libkernel/libkernel.h>
#include <platform/x86/idt.h>

static driver_desc_t _keyboard_driver_info();
static key_t _kbdriver_apply_modifiers(key_t key);

static void _kbdriver_notification(uint32_t msg, uint32_t param)
{
    if (msg == DM_NOTIFICATION_DEVFS_READY) {
        if (generic_keyboard_create_devfs() < 0) {
            kpanic("Can't init keyboard in devfs");
        }
    }
}

bool kbdriver_install()
{
    driver_install(_keyboard_driver_info(), "kbd86");
    return true;
}

void kbdriver_run()
{
    set_irq_handler(IRQ1, keyboard_handler);
    generic_keyboard_init();
}

void keyboard_handler()
{
    uint32_t scancode = (uint32_t)port_byte_in(0x60);
    generic_emit_key_set1(scancode);
}