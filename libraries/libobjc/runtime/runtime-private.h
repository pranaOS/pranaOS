/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef runtime_private_h
#define runtime_private_h

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#ifdef __LP64__
#   define WORD_SHIFT 3UL
#   define WORD_MASK 7UL
#   define WORD_BITS 64
#else
#   define WORD_SHIFT 2UL
#   define WORD_MASK 3UL
#   define WORD_BITS 32
#endif

/**
 * @brief word align
 * 
 * @param x 
 * @return uint32_t 
 */
static inline uint32_t word_align(uint32_t x) {
    return (x + WORD_MASK) & ~WORD_MASK;
}

static inline size_t word_align(size_t x) {
    return (x + WORD_MASK) & ~WORD_MASK;
}

#if __LP64__
typedef uint32_t mask_t;  
#else
typedef uint16_t mask_t;
#endif
typedef uintptr_t cache_key_t;

static inline void* memdup(const void *mem, size_t len) {
    void *dup = malloc(len);
    memcpy(dup, mem, len);
    return dup;
}

#endif /* runtime_private_h */
