/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef LIBKERNEL_VSPRINTF_H
#define LIBKERNEL_VSPRINTF_H

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

/**
 * @brief vscnprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

/**
 * @brief snprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int snprintf(char *buf, size_t size, const char *fmt, ...);

/**
 * @brief scnprintf
 * 
 * @param buf 
 * @param size 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int scnprintf(char *buf, size_t size, const char *fmt, ...);

/**
 * @brief vsprintf
 * 
 * @param buf 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vsprintf(char *buf, const char *fmt, va_list args);

/**
 * @brief sprintf
 * 
 * @param buf 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int sprintf(char *buf, const char *fmt, ...);

/**
 * @brief vsscanf
 * 
 * @param buf 
 * @param fmt 
 * @param args 
 * @return int 
 */
int vsscanf(const char *buf, const char *fmt, va_list args);

/**
 * @brief sscanf
 * 
 * @param buf 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int sscanf(const char *buf, const char *fmt, ...);


#endif 