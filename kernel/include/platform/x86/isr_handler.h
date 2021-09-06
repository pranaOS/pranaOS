/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <drivers/x86/display.h>
#include <libkern/types.h>
#include <platform/x86/idt.h>

void isr_handler(trapframe_t* tf);
void isr_standart_handler(trapframe_t* tf);
uint32_t rcr2();