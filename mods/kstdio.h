/**
 * @file kstdio.h
 * @author Krisna Pranav
 * @brief kstdio
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#ifdef __pranaos__
#ifdef KERNEL
#include <kernel/kstdio.h>
#else
#include <mods/types.h>
#include <stdarg.h>
extern "C"
{
    void dbgputstr(char const*, size_t);
    int sprintf(char* buf, char const* fmt, ...) __attribute__((format(printf, 2, 3)));
    int snprintf(char* buffer, size_t, char const* fmt, ...) __attribute__((format(printf, 3, 4)));
}
#endif
#else
#include <stdio.h>
inline void dbgputstr(char const* characters, size_t length)
{
    fwrite(characters, 1, length, stderr);
}
#endif
template <size_t N>
inline void dbgputstr(char const (&array)[N])
{
    return ::dbgputstr(array, N);
}
