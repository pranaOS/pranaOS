/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

#include <stddef.h>

#ifndef __cplusplus 
extern "C" {
#endif 

/**
 * @brief string length functions
 * 
 * @param str 
 * @return size_t 
 */
size_t strlen(const char* str);
size_t strnlen(const char* s, size_t maxlen);

#ifndef __cplusplus
}

#endif 