/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_DRIVERS_X86_IDE_H
#define _KERNEL_DRIVERS_X86_IDE_H

// includes
#include <drivers/x86/ata.h>
#include <libkernel/types.h>
#include <platform/x86/idt.h>

void ide_install();
void ide_find_devices(device_t *t_device);

#endif 