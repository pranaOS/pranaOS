/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define KERNEL_PM_BASE 0x80100000
#define KERNEL_BASE 0xc0000000
#define KMALLOC_BASE (KERNEL_BASE + 0x400000)

extern struct memory_map* arm_memmap;