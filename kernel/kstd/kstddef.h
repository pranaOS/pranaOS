/**
 * @file kstddef.h
 * @author Krisna Pranav
 * @brief KSTD_DEF
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/**
 * @brief Registers
 * 
 */
struct Registers {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx;
    unsigned int num, err_code;
};

void *operator new(size_t size);
void *operator new(size_t size, void* ptr);
void *operator new[](size_t size);
void *operator new[](size_t size, void* ptr);