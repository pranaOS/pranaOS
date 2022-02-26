/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _osruntime_h_
#define _osruntime_h_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

class osRuntime {
public:
    /**
     * @brief os malloc
     * 
     * @param size 
     * @return void* 
     */
    static void* osMalloc(size_t size);

    /**
     * @brief osfree
     * 
     * @param addr 
     */
    static void osFree(void* addr);
};

/**
 * @brief osReturn
 * 
 */
typedef int osReturn;

/**
 * @brief os return signals
 * 
 */
#define osReturnSuccess  0
#define osReturnTimeout -1
#define osReturnFailed  -2
#define osReturnError   -3

#endif /* osruntime_h */
