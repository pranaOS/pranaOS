/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Array.h>
#include <base/Assertions.h>
#include <base/Span.h>
#include <base/Types.h>
#include <base/Vector.h>

namespace Base {

namespace {
const static void* bitap_bitwise(const void* haystack, size_t haystack_length, const void* needle, size_t needle_length)
{
    VERIFY(needle_length < 32);

    u64 lookup = 0xfffffffe;

    constexpr size_t mask_length = (size_t)((u8)-1) + 1;
    u64 needle_mask[mask_length];

    for (size_t i = 0; i < mask_length; ++i)
        needle_mask[i] = 0xffffffff;

    for (size_t i = 0; i < needle_length; ++i)
        needle_mask[((const u8*)needle)[i]] &= ~(0x00000001 << i);

    for (size_t i = 0; i < haystack_length; ++i) {
        lookup |= needle_mask[((const u8*)haystack)[i]];
        lookup <<= 1;

        if (!(lookup & (0x00000001 << needle_length)))
            return ((const u8*)haystack) + i - needle_length + 1;
    }

    return nullptr;
}
}

}