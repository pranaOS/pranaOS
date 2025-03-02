/*
 * Copyright (c) 2021 - 2025, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _objc_h_
#define _objc_h_
#undef KERNEL
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "runtime.h"

/**
 * @brief objc_class[isa, superclass, name, version, instance_size, info]
 * 
 */
struct objc_class {
    struct objc_class* isa;
    struct objc_class* superclass;

    const char* name;

    long version;
    long instance_size;

    unsigned long info;
};

/**
 * @brief objc_object
 * 
 */
struct objc_object {
    Class isa;
};

/**
 * @brief objc_selector
 * 
 */
struct objc_selector {
    // 
};

/**
 * @brief NS INTEGERS
 * 
 */
#undef NSInteger
#undef NSUInteger
typedef          long NSInteger;
typedef unsigned long NSUInteger;

#define NSIntegerMax    LONG_MAX
#define NSIntegerMin    LONG_MIN
#define NSUIntegerMax   ULONG_MAX

/**
 * @brief bool -> BOOL
 * 
 */
#undef BOOL
#define OBJC_BOOL_IS_BOOL 1
typedef bool  BOOL;

/**
 * @brief debug
 * 
 */
#ifdef DEBUG
#define DBG(x...) kprintf(x)
#else
#define DBG(x...)
#endif

/**
 * @brief ultDebug
 * 
 */
#ifdef ULTDEBUG
#define ULTDBG(x...) kprintf(x)
#else
#define ULTDBG(x...)
#endif

/**
 * @brief has objc_bool??
 * 
 */
#if __has_feature(objc_bool)
#define YES   __objc_yes
#define NO    __objc_no
#else
#define YES   (BOOL)1
#define NO    (BOOL)0
#endif

#define nil (id)0

#define Nil (Class)0

#ifndef __OBJC__

typedef struct objc_object Protocol;
#else 

#endif

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

#define UNIMPLEMENTED(x) void x() { ULTDBG(__func__); }

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief kprintf
     * 
     * @param ... 
     */
    extern void kprintf(const char*, ...);
    
    /**
     * @brief panic
     * 
     * @param ... 
     */
    extern void panic(const char*, ...);

    /**
     * @brief putchar
     * 
     * @return int 
     */
    extern int  putchar(int);

    /**
     * @brief puts
     * 
     * @return int 
     */
    extern int  puts(const char*);

    /**
     * @brief strcmp
     * 
     * @return int 
     */
    extern int strcmp(const char*, const char*);

    extern void* malloc(size_t size);
    extern void free(void* ptr);
    extern size_t strlen(const char*);
    extern int strcmp(const char* str1, const char* str2);
    extern int strncmp(const char *s1, const char *s2, size_t n);
    extern size_t strlcpy(char * dst, const char * src, size_t maxlen);
    extern char * strncpy(char * dst, const char * src, size_t maxlen);
    extern char * strcpy(char *dest, const char *src);
    extern char * strcat(char *dest, const char *src);
    
    extern void* memcpy  (void* dstptr, const void* srcptr, size_t size);
    extern void* memmove (void* dstptr, const void* srcptr, size_t size);
    extern void* memset  (void* bufptr, int  value,  size_t size);
    extern void* memchr  (const void* s1, int         c,      size_t size);
    extern int   memcmp  (const void* s1, const void* s2,     size_t size);
#ifdef __cplusplus
}
#endif

#endif /* objc_h */
