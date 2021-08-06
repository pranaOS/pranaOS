/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_DRIVERS_X86_BGA_H
#define _KERNEL_DRIVERS_X86_BGA_H

// includes
#include <drivers/driver_manager.h>
#include <drivers/x86/display.h>
#include <libkernel/types.h>
#include <mem/kmalloc.h>
#include <platform/x86/port.h>

void bga_install();
void bga_init();
void bga_set_resolution(uint16_t width, uint16_t height);

#endif 