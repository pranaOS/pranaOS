/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <drivers/x86/display.h>
#include <platform/x86/idt.h>

void irq_handler(trapframe_t* tf);
void irq_empty_handler();