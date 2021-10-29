/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <kernel/tty.h>
 
void kernel(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
}