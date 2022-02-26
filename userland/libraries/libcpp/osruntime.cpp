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

const char *osReturnStrings[4] = {"Success", "Timeout", "Failure", "Error"};

struct _mhead {
    size_t mlen;
    char dat[0] __attribute__((aligned(16)));
};

void* osRuntime::osMalloc(size_t size) {
    struct _mhead *mem;
    size_t memsize = sizeof(*mem) + size;
    
    if (size == 0) {
        return 0;
    }
    
    mem = (struct _mhead *) kalloc_((uint32_t)memsize);
    if (!mem) {
        return 0;
    }
    
    mem->mlen = memsize;
    bzero(mem->dat, size);
    
    return mem->dat;
}
