/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/CharacterTypes.h>
#include <base/Format.h>
#include <base/GenericLexer.h>
#include <base/String.h>
#include <base/StringBuilder.h>
#include <base/kstdio.h>

#if defined(__pranaos__) && !defined(KERNEL)
#    include <pranaos.h>
#endif

#ifdef KERNEL
#    include <kernel/Process.h>
#    include <kernel/Thread.h>
#else
#    include <stdio.h>
#    include <string.h>
#endif

namespace Base {

namespace {

static constexpr size_t use_next_index = NumericLimits<size_t>::max();

static constexpr size_t convert_unsigned_to_string(u64 value, Array<u8, 128>& buffer, u8 base, bool upper_case)
{
    VERIFY(base >= 2 && base <= 16);

    constexpr const char* lowercase_lookup = "0123456789abcdef";
    constexpr const char* uppercase_lookup = "0123456789ABCDEF";

    if (value == 0) {
        buffer[0] = '0';
        return 1;
    }

    size_t used = 0;
    while (value > 0) {
        if (upper_case)
            buffer[used++] = uppercase_lookup[value % base];
        else
            buffer[used++] = lowercase_lookup[value % base];

        value /= base;
    }

    for (size_t i = 0; i < used / 2; ++i)
        swap(buffer[i], buffer[used - i - 1]);

    return used;
}


}

}