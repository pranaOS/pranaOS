/**
 * @file kstdio.h
 * @author Krisna Pranav
 * @brief kstdio
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifdef __pranaos__
#    ifdef KERNEL
#        include <kernel/kstdio.h>
#    else
#        include "types.h"
#        include <stdarg.h>
extern "C" {
    int vdbgprintf(const char* fmt, va_list);
    int dbgprintf(const char* fmt, ...);
    int dbgputstr(const char*, ssize_t);
    int sprintf(char* buf, const char* fmt, ...);
    int snprintf(char* buffer, size_t, const char* fmt, ...);
}
#    endif
#else
#    include <stdio.h>
#    define kprintf printf
#    define dbgprintf(...) fprintf(stderr, __VA_ARGS__)
/**
 * @param characters 
 * @param length 
 * @return int 
 */
inline int dbgputstr(const char* characters, ssize_t length) {
    fwrite(characters, 1, length, stderr);
    return 0;
}
#endif

/**
 * 
 * @tparam N 
 * @return int 
 */
template<size_t N>
inline int dbgputstr(const char (&array)[N]) {
    return ::dbgputstr(array, N);
}
