/**
 * @file hex.h
 * @author Krisna Pranav
 * @brief hex
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
} // namespace Mods

using Mods::decode_hex;