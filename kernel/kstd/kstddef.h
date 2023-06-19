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

/**
 * @brief STR_HELPER, STR
 * 
 */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/**
 * @brief Registers
 * 
 */
struct Registers {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int num, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

/**
 * @brief new operator
 * 
 * @param size 
 * @return void* 
 */
void *operator new(size_t size);
void *operator new(size_t size, void* ptr);
void *operator new[](size_t size);
void *operator new[](size_t size, void* ptr);

/**
 * @brief delete operator
 * 
 * @param p 
 */
void operator delete(void *p) noexcept;
void operator delete(void *p, size_t size) noexcept;
void operator delete[](void *p) noexcept;
void operator delete[](void *p, size_t size) noexcept;