/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <drivers/x86/ata.h>
#include <libkern/types.h>
#include <platform/x86/idt.h>

void ide_install();
void ide_find_devices(device_t* t_device);
