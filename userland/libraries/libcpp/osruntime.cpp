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
    /**
     * @brief kalloc
     * 
     * @param size 
     * @return void* 
     */
    void* kalloc_(uint32_t size);

    /**
     * @brief free
     * 
     * @param data 
     * @param size 
     */
    void free_(void* data, uint32_t size);

    /**
     * @brief panic
     * 
     * @param fmt 
     * @param ... 
     */
    void panic(const char* fmt, ...);
}

const char *osReturnStrings[4] = {"Success", "Timeout", "Failure", "Error"};

struct _mhead {
    /**
     * @brief mlen
     * 
     */
    size_t mlen;

    /**
     * @brief dat
     * 
     */
    char dat[0] __attribute__((aligned(16)));
};

/**
 * @brief osMalloc
 * 
 * @param size 
 * @return void* 
 */
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

/**
 * @brief os free
 * 
 * @param addr 
 */
void osRuntime::osFree(void* addr) {
    struct _mhead* header;
    if (!addr) {
        return;
    }
    
    header = (struct _mhead*) addr;
    header--;
    
    memset(header, 0xbb, header->mlen);
    free_(header, (uint32_t)header->mlen);
}

/**
 * @brief operator new 
 * 
 * @param size 
 * @return void* 
 */
void * operator new(size_t size) noexcept {
    void * result;
    
    return (void *) osRuntime::osMalloc(size);
    return result;
}

/**
 * @brief delete 
 * 
 * @param addr 
 */
void operator delete(void* addr) noexcept {
    osRuntime::osFree(addr);
    return;
}
