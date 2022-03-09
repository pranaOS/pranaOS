/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef PRANAOS_LOCKING_SPINLOCK
#define PRANAOS_LOCKING_SPINLOCK

#define barrier() asm volatile("" \
							   :  \
							   :  \
							   : "memory")

#define cpu_relax() asm volatile("pause\n" \
								 :         \
								 :         \
								 : "memory")

/**
 * @brief xchg_8
 * 
 * @param ptr 
 * @param x 
 * @return unsigned short 
 */
static inline unsigned short xchg_8(void *ptr, unsigned char x) {
	__asm__ __volatile__("xchgb %0,%1"
						 : "=r"(x)
						 : "m"(*(volatile unsigned char *)ptr), "0"(x)
						 : "memory");

	return x;
}

#define SPINLOCK_UNLOCKED 0
#define SPINLOCK_LOCK 1

#endif