#pragma once

#include <types.h>
#include <stddef.h>

/**
 * @brief kmmap args
 * 
 */
struct kmmap_args {
    void *addr;
    size_t len;
    int prot;
    int flags;
    int flides;
    off_t off;
};