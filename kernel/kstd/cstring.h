/**
 * @file cstring.h
 * @author Krisna Pranav
 * @brief cstring
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav 
 * 
 */

#pragma once

#include "types.h"

#ifdef PRANAOS_KERNEL

char *strcat(char *dest, const char *src);
bool strcmp(const char *str1, const char *str2)
void* memcpy_uint32(uint32_t* d, uint32_t* s, size_t n);

#endif