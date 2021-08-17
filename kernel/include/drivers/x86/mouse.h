/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_DRIVERS_X86_MOUSE_H
#define _KERNEL_DRIVERS_X86_MOUSE_H

#include <drivers/generic/mouse.h>
#include <drivers/x86/ata.h>
#include <libkernel/types.h>
#include <platform/x86/idt.h>

bool mouse_install();

#endif 
