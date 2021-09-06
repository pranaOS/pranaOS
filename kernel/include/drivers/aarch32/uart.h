/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>

#define COM1 0x1c090000

void uart_setup();
void uart_remap();
int uart_write(int port, uint8_t data);
int uart_read(int port, uint8_t* data);