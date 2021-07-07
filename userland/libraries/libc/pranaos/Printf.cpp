/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <ctype.h>

#include <skift/NumberFormatter.h>
#include <skift/Printf.h>
#include <string.h>

int __printf_formate_binary(printf_info_t *info, va_list *va)
{
    unsigned long v = va_arg(*va, unsigned long);

    char buffer[65] = {};

    NumberFormater format = FORMAT_BINARY;
    format.padded_with_zero = false;

    format_uint(format, v, buffer, 65);

    PRINTF_PADDING(buffer, PFALIGN_RIGHT);

    for (int i = 0; buffer[i]; i++)
    {
        PRINTF_APPEND(buffer[i]);
    }

    PRINTF_PADDING(buffer, PFALIGN_LEFT);

    return info->written;
}

int __printf_formate_octal(printf_info_t *info, va_list *va)
{
    unsigned long v = va_arg(*va, unsigned long);

    char buffer[65] = {};

    NumberFormater format = FORMAT_OCTAL;
    format.padded_with_zero = false;

    format_uint(format, v, buffer, 65);

    PRINTF_PADDING(buffer, PFALIGN_RIGHT);

    for (int i = 0; buffer[i]; i++)
    {
        PRINTF_APPEND(buffer[i]);
    }

    PRINTF_PADDING(buffer, PFALIGN_LEFT);

    return info->written;
}

int __printf_formate_decimal(printf_info_t *info, va_list *va)
{
    long v = va_arg(*va, long);

    char buffer[65] = {};

    NumberFormater format = FORMAT_DECIMAL;
    format.padded_with_zero = false;

    format_int(format, v, buffer, 65);

    PRINTF_PADDING(buffer, PFALIGN_RIGHT);

    for (int i = 0; buffer[i]; i++)
    {
        PRINTF_APPEND(buffer[i]);
    }

    PRINTF_PADDING(buffer, PFALIGN_LEFT);

    return info->written;
}

int __printf_formate_decimal_unsigned(printf_info_t *info, va_list *va)
{
    unsigned long v = va_arg(*va, unsigned long);

    char buffer[65] = {};

    NumberFormater format = FORMAT_DECIMAL;
    format.padded_with_zero = false;

    format_uint(format, v, buffer, 65);

    PRINTF_PADDING(buffer, PFALIGN_RIGHT);

    for (int i = 0; buffer[i]; i++)
    {
        PRINTF_APPEND(buffer[i]);
    }

    PRINTF_PADDING(buffer, PFALIGN_LEFT);

    return info->written;
}
