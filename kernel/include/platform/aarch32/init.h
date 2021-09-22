/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>

void platform_init_boot_cpu();
void platform_setup_boot_cpu();
void platform_setup_secondary_cpu();
void platform_drivers_setup();