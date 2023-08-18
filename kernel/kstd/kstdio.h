/**
 * @file kstdio.h
 * @author Krisna Pranav
 * @brief KSTDIO
 * @version 6.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kstddef.h"
#include <api/stdarg.h>

#ifdef DEBUG
#define ASSERT(cond) \
if(!(cond)) { \
  PANIC("ASSERTION FAILED:", __FILE__ " at line " STR(__LINE__)); \
}
#else
#define ASSERT(cond) \
if(!(cond)) { \
  printf("[KERNEL] CRITICAL: Assertion failed: %s\n", __FILE__ " at line " STR(__LINE__)); \
}
#endif

/**
 * @brief putch
 * 
 * @param c 
 */
void putch(char c);

/**
 * @brief serial putch
 * 
 * @param c 
 */
void serial_putch(char c);

/**
 * @brief vprintf
 * 
 * @param fmt 
 * @param list 
 */
void vprintf(const char* fmt, va_list list);

/**
 * @brief printf
 * 
 * @param fmt 
 * @param ... 
 */
void printf(const char* fmt, ...);

/**
 * @brief print
 * 
 * @param str 
 */
void print(const char* str);

/**
 * @brief PANIC
 * 
 * @param error 
 * @param msg 
 * @param ... 
 */
[[noreturn]] void PANIC(const char *error, const char *msg, ...);

/**
 * @brief clearScreen, setup_tty
 * 
 */
void clearScreen();
void setup_tty();