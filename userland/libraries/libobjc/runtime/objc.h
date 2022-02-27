/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef objc_h
#define objc_h
#undef KERNEL
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "runtime.h"

struct objc_class {
    /**
     * @brief isa
     * 
     */
    struct objc_class* isa;

    /**
     * @brief super class
     * 
     */
    struct objc_class* superclass;

    /**
     * @brief name
     * 
     */
    const char* name;

    /**
     * @brief version, instance_size
     * 
     */
    long version;
    long instance_size;

    /**
     * @brief info
     * 
     */
    unsigned long info;
};

struct objc_object {
    /**
     * @brief isa 
     * 
     */
    Class isa;
};

struct objc_selector {
    // 
};

#undef NSInteger
#undef NSUInteger
typedef long NSInteger;
typedef long NSUInteger;

/**
 * @brief nsIntegers
 * 
 */
#define NSIntegerMax    LONG_MAX
#define NSIntegerMin    LONG_MIN
#define NSUIntegerMax   ULONG_MAX

/**
 * @brief bool -> BOOL
 * 
 */
#undef BOOL
#define OBJC_BOOL_IS_BOOL 1
typedef bool BOOL;

#ifndef __cplusplus
extern "C" {
#endif
    extern void kprintf(const char*, ...);
    extern void panic(const char*, ...);
    extern int putchar(int);
#ifndef __cplusplus
}
#endif

#endif /* objc_h */
