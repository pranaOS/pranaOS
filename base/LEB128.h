/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NumericLimits.h>
#include <base/Stream.h>
#include <base/Types.h>

namespace Base {

struct LEB128 {
    template<typename StreamT, typename ValueType = size_t>
    static bool read_unsigned(StreamT& stream, ValueType& result)
    {
        [[maybe_unused]] size_t backup_offset = 0;
        if constexpr (requires { stream.offset(); })
            backup_offset = stream.offset();
        InputStream& input_stream { stream };

        result = 0;
        size_t num_bytes = 0;
        while (true) {
            if (input_stream.unreliable_eof()) {
                if constexpr (requires { stream.seek(backup_offset); })
                    stream.seek(backup_offset);
                input_stream.set_fatal_error();
                return false;
            }
            u8 byte = 0;
            input_stream >> byte;
            if (input_stream.has_any_error())
                return false;

            ValueType masked_byte = byte & ~(1 << 7);
            const bool shift_too_large_for_result = (num_bytes * 7 > sizeof(ValueType) * 8) && (masked_byte != 0);
            const bool shift_too_large_for_byte = ((masked_byte << (num_bytes * 7)) >> (num_bytes * 7)) != masked_byte;
            if (shift_too_large_for_result || shift_too_large_for_byte)
                return false;

            result = (result) | (masked_byte << (num_bytes * 7));
            if (!(byte & (1 << 7)))
                break;
            ++num_bytes;
        }

        return true;
    }

};

}