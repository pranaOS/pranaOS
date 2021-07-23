/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Format.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>
#include <stdarg.h>

#ifdef __pranaos__
extern "C" size_t strlen(const char*);
#else
#    include <string.h>
#endif

namespace PrintfImplementation {

template<typename PutChFunc, typename T>
ALWAYS_INLINE int print_hex(PutChFunc putch, char*& bufptr, T number, bool upper_case, bool alternate_form, bool left_pad, bool zero_pad, u8 field_width)
{
    int ret = 0;

    int digits = 0;
    for (T n = number; n > 0; n >>= 4)
        ++digits;
    if (digits == 0)
        digits = 1;

    if (left_pad) {
        int stop_at = field_width - digits;
        if (alternate_form)
            stop_at -= 2;

        while (ret < stop_at) {
            putch(bufptr, ' ');
            ++ret;
        }
    }

    if (alternate_form) {
        putch(bufptr, '0');
        putch(bufptr, 'x');
        ret += 2;
        field_width += 2;
    }

    if (zero_pad) {
        while (ret < field_width - digits) {
            putch(bufptr, '0');
            ++ret;
        }
    }

    if (number == 0) {
        putch(bufptr, '0');
        ++ret;
    } else {
        u8 shift_count = digits * 4;
        while (shift_count) {
            constexpr const char* printf_hex_digits_lower = "0123456789abcdef";
            constexpr const char* printf_hex_digits_upper = "0123456789ABCDEF";

            shift_count -= 4;
            putch(bufptr,
                  upper_case
                  ? printf_hex_digits_upper[(number >> shift_count) & 0x0f]
                  : printf_hex_digits_lower[(number >> shift_count) & 0x0f]);
            ++ret;
        }
    }

    return ret;
}

}