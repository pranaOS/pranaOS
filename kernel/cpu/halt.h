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

static __inline void disable_interrupt() {
    __asm__ __volatile__("cli");
}

static __inline unsigned char inportb(unsigned short _port) {
    unsigned char rv;
    asm volatile("inb %1, %0"
                 : "=a"(rv)
                 : "dN"(_port));
}

static __inline void outportw(unsigned short _port, unsigned short _data) {
    asm volatile("outw %1, %0"
                            :
                            : "dn"(_port), "a"(_data));
}

void cpuid(int code, uint32_t *a, uint32_t *b);
const char *get_cpu_vender();