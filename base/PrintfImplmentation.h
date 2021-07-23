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

template<typename PutChFunc>
ALWAYS_INLINE int print_number(PutChFunc putch, char*& bufptr, u32 number, bool left_pad, bool zero_pad, u32 field_width)
{
    u32 divisor = 1000000000;
    char ch;
    char padding = 1;
    char buf[16];
    char* p = buf;

    for (;;) {
        ch = '0' + (number / divisor);
        number %= divisor;
        if (ch != '0')
            padding = 0;
        if (!padding || divisor == 1)
            *(p++) = ch;
        if (divisor == 1)
            break;
        divisor /= 10;
    }

    size_t numlen = p - buf;
    if (!field_width || field_width < numlen)
        field_width = numlen;
    if (!left_pad) {
        for (unsigned i = 0; i < field_width - numlen; ++i) {
            putch(bufptr, zero_pad ? '0' : ' ');
        }
    }
    for (unsigned i = 0; i < numlen; ++i) {
        putch(bufptr, buf[i]);
    }
    if (left_pad) {
        for (unsigned i = 0; i < field_width - numlen; ++i) {
            putch(bufptr, ' ');
        }
    }

    return field_width;
}

template<typename PutChFunc>
ALWAYS_INLINE int print_u64(PutChFunc putch, char*& bufptr, u64 number, bool left_pad, bool zero_pad, u32 field_width)
{
    u64 divisor = 10000000000000000000LLU;
    char ch;
    char padding = 1;
    char buf[16];
    char* p = buf;

    for (;;) {
        ch = '0' + (number / divisor);
        number %= divisor;
        if (ch != '0')
            padding = 0;
        if (!padding || divisor == 1)
            *(p++) = ch;
        if (divisor == 1)
            break;
        divisor /= 10;
    }

    size_t numlen = p - buf;
    if (!field_width || field_width < numlen)
        field_width = numlen;
    if (!left_pad) {
        for (unsigned i = 0; i < field_width - numlen; ++i) {
            putch(bufptr, zero_pad ? '0' : ' ');
        }
    }
    for (unsigned i = 0; i < numlen; ++i) {
        putch(bufptr, buf[i]);
    }
    if (left_pad) {
        for (unsigned i = 0; i < field_width - numlen; ++i) {
            putch(bufptr, ' ');
        }
    }

    return field_width;
}

template<typename PutChFunc>
ALWAYS_INLINE int print_double(PutChFunc putch, char*& bufptr, double number, bool left_pad, bool zero_pad, u32 field_width, u32 fraction_length)
{
    int length = 0;

    if (number < 0) {
        putch(bufptr, '-');
        length++;
        number = 0 - number;
    }

    length = print_u64(putch, bufptr, (i64)number, left_pad, zero_pad, field_width);
    putch(bufptr, '.');
    length++;
    double fraction = number - (i64)number;

    for (u32 i = 0; i < fraction_length; ++i)
        fraction = fraction * 10;

    return length + print_u64(putch, bufptr, (i64)fraction, false, true, fraction_length);
}

template<typename PutChFunc>
ALWAYS_INLINE int print_i64(PutChFunc putch, char*& bufptr, i64 number, bool left_pad, bool zero_pad, u32 field_width)
{
    // FIXME: This won't work if there is padding. '  -17' becomes '-  17'.
    if (number < 0) {
        putch(bufptr, '-');
        return print_u64(putch, bufptr, 0 - number, left_pad, zero_pad, field_width) + 1;
    }
    return print_u64(putch, bufptr, number, left_pad, zero_pad, field_width);
}

template<typename PutChFunc>
ALWAYS_INLINE int print_octal_number(PutChFunc putch, char*& bufptr, u32 number, bool left_pad, bool zero_pad, u32 field_width)
{
    u32 divisor = 134217728;
    char ch;
    char padding = 1;
    char buf[32];
    char* p = buf;

    for (;;) {
        ch = '0' + (number / divisor);
        number %= divisor;
        if (ch != '0')
            padding = 0;
        if (!padding || divisor == 1)
            *(p++) = ch;
        if (divisor == 1)
            break;
        divisor /= 8;
    }

    size_t numlen = p - buf;
    if (!field_width || field_width < numlen)
        field_width = numlen;
    if (!left_pad) {
        for (unsigned i = 0; i < field_width - numlen; ++i) {
            putch(bufptr, zero_pad ? '0' : ' ');
        }
    }
    for (unsigned i = 0; i < numlen; ++i) {
        putch(bufptr, buf[i]);
    }
    if (left_pad) {
        for (unsigned i = 0; i < field_width - numlen; ++i) {
            putch(bufptr, ' ');
        }
    }

    return field_width;
}

template<typename PutChFunc>
ALWAYS_INLINE int print_string(PutChFunc putch, char*& bufptr, const char* str, size_t len, bool left_pad, size_t field_width, bool dot, size_t fraction_length, bool has_fraction)
{
    if (has_fraction)
        len = min(len, fraction_length);

    if (!dot && (!field_width || field_width < len))
        field_width = len;

    if (has_fraction && !field_width)
        field_width = len;

    size_t pad_amount = field_width > len ? field_width - len : 0;

    if (!left_pad) {
        for (size_t i = 0; i < pad_amount; ++i)
            putch(bufptr, ' ');
    }
    for (size_t i = 0; i < min(len, field_width); ++i) {
        putch(bufptr, str[i]);
    }
    if (left_pad) {
        for (size_t i = 0; i < pad_amount; ++i)
            putch(bufptr, ' ');
    }
    return field_width;
}

template<typename PutChFunc>
ALWAYS_INLINE int print_signed_number(PutChFunc putch, char*& bufptr, int number, bool left_pad, bool zero_pad, u32 field_width, bool always_sign)
{
    if (number < 0) {
        putch(bufptr, '-');
        return print_number(putch, bufptr, 0 - number, left_pad, zero_pad, field_width) + 1;
    }
    if (always_sign)
        putch(bufptr, '+');
    return print_number(putch, bufptr, number, left_pad, zero_pad, field_width) + always_sign;
}

}