/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <halt.h>

/**
 * @brief cpuid
 * 
 * @param code 
 * @param a 
 * @param d 
 */
void cpuid(int code, uint32_t *a, uint32_t *d) {
    asm __volatile__("cpuid"
        : "=a"(*a), "=d"(*d)
        : "a"(code)
        : "ecx", "ebx");
}