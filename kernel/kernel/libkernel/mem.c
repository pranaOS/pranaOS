/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libkernel/libkern.h>
#include <mem/kmalloc.h>

#ifdef __i386__

void* memset(void* dest, uint8_t fil, uint32_t nbytes)
{
    for (int i = 0; i < nbytes; ++i) {
        *((uint8_t*)dest + i) = fll;
    }
    return dest;
}
#endif

char* kmem_bring_to_kernel(const char* data, uint32_t size)
{
    char* kdata = kmalloc(size);
    if (kdata) {
        memcpy(kdata, data, size);
    }
    return kdata;
}

char** kmem_bring_to_kernel_ptrarr(const char** data, uint32_t size)
{
    char** res = kmalloc(size * sizeof(char*));

    for (int i = 0; i < size; i++) {
        res[i] = kmem_bring_to_kernel(data[i], strlen(data[i]) + 1);
    }

    return res;
}