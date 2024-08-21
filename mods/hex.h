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

#include "forward.h"
#include <mods/byte_buffer.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/string.h>
#include <mods/string_view.h>

namespace Mods 
{

    /**
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> decode_hex(const StringView&);

    /**
     * @return String 
     */
    String encode_hex(ReadonlyBytes);

} // namespace Mods


// using mods
using Mods::decode_hex;
using Mods::encode_hex;
