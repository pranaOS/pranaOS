/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/cdefs.h>
#include <stddef.h>
#include <stdint.h>

static __inline void io_wait() {
	__asm__ __volatile__("outb %%al, $0x80"
						 :
						 : "a"(0));
}

static __inline void enable_interrupt() {
    __asm__ __volatile__("sti");
}

static inline void disable_interrupt() {
    __asm__ __volatile__("cli");
}
