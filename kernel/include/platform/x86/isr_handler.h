/*
 * Copyright (c) 2021, OliviaCE
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_X86_ISR_HANDLER_H
#define _KERNEL_PLATFORM_X86_ISR_HANDLER_H

// includes
#include <drivers/x86/display.h>
#include <libkernel/types.h>
#include <platform/x86/idt.h>

void isr_handler(trapframe_t *tf);
void isr_standart_handler(trapframe_t *tf);
uint32_t rcr2();

#endif
