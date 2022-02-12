/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief vsnprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);