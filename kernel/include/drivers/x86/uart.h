/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_DRIVERS_X86_UART_H
#define _KERNEL_DRIVERS_X86_UART_H

#include <libkernel/types.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

void uart_setup();
int uart_write(int port, uint8_t data);
int uart_read(int port, uint8_t* data);

#endif //_KERNEL_DRIVERS_X86_UART_H