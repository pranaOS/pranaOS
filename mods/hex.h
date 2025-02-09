/**
 * @file hex.h
 * @author Krisna Pranav
 * @brief hex
 * @version 6.0
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/error.h>
#include <mods/stringview.h>

#ifdef KERNEL
#include <kernel/kstring.h>
#else
#include <mods/string.h>
#endif

namespace Mods
{
    /**
     * @param digit 
     * @return constexpr u8 
     */
    constexpr u8 decode_hex_digit(char digit)
    {
        if(digit >= '0' && digit <= '9')
            return digit - '0';
        if(digit >= 'a' && digit <= 'f')
            return 10 + (digit - 'a');
        if(digit >= 'A' && digit <= 'F')
            return 10 + (digit - 'A');
        return 255;
    }

    /**
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> decode_hex(StringView);

    #ifdef KERNEL
    ErrorOr<NonnullOwnPtr<Kernel::KString>> encode_hex(ReadonlyBytes);
    #else
    String encode_hex(ReadonlyBytes);
    #endif
} // namespace Mods

using Mods::decode_hex;
using Mods::decode_hex_digit;
using Mods::encode_hex;
