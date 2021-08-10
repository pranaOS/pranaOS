/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_DRIVERS_AARCH32_PL050_H
#define _KERNEL_DRIVERS_AARCH32_PL050_H

// includes
#include <drivers/driver_manager.h>
#include <platform/aarch32/target/cortex-a15/device_settings.h>
#include <libkernel/types.h>
#include <libkernel/mask.h>

struct pl050_registers {
    uint32_t cr; 
    uint32_t stat; 
    uint32_t data; 
    uint32_t clk; 
    uint32_t ir;
};
typedef struct pl050_registers pl050_registers_t;

void pl050_keyboard_install();
void pl050_mouse_install();

#endif 