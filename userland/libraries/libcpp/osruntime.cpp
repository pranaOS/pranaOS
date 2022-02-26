/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "osruntime.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

extern "C" {
    void* kalloc_(uint32_t size);
    void free_(void* data, uint32_t size);
    void panic(const char* fmt, ...);
}
