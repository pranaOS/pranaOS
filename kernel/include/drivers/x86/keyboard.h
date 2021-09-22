/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <drivers/driver_manager.h>
#include <drivers/generic/keyboard.h>

void keyboard_handler();
bool kbdriver_install();
void kbdriver_run();

uint32_t kbdriver_get_last_key();
void kbdriver_discard_last_key();