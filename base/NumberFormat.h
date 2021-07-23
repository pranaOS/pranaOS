/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/String.h>

namespace Base {

static String number_string_with_one_decimal(u64 number, u64 uint, const char* suffix)
{
    int decimal = (number % uint) * 10 / uint;
    return String::formatted("{}.{} {}", number / uint, decimal, suffix);
}

static inline String human_readable_size_long(u64 size)
{
    if (size < 1 * KiB)
        return String::formatted("{} bytes", size);
    else
        return String::formatted("{} ({} bytes)", human_readable_size(size), size);
}

}