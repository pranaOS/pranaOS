/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <halt.h>

/**
 * @brief i86 cpu venodor
 * 
 * @return char* 
 */
char *i86_cpu_vender() {
	static char vender[32] = {0};
	__asm__ __volatile__(
		"mov $0, %%eax;				\n"
		"cpuid;								\n"
		"lea (%0), %%eax;			\n"
		"mov %%ebx, (%%eax);		\n"
		"mov %%edx, 0x4(%%eax);\n"
		"mov %%ecx, 0x8(%%eax)	\n"
		: "=m"(vender));
	return vender;
}

/**
 * @brief cpuid
 * 
 * @param code 
 * @param a 
 * @param d 
 */
void cpuid(int code, uint32_t *a, uint32_t *d) {
	asm volatile("cpuid"
				 : "=a"(*a), "=d"(*d)
				 : "a"(code)
				 : "ecx", "ebx");
}