/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/prelude.h>

struct NumberFormater
{
    int base;
    int after_point;
    bool padded_with_zero;
    bool captialized;
};

size_t format_uint(NumberFormater formater, unsigned long value, char *str, size_t size);

size_t format_int(NumberFormater formater, long value, char *str, size_t size);

#ifndef __KERNEL__

size_t format_double(NumberFormater formater, double value, char *str, size_t size);

#endif