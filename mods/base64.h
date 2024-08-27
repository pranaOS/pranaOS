/**
 * @file base64.h
 * @author Krisna Pranav
 * @brief Base64
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "byte_buffer.h"
#include "string.h"
#include "string_view.h"
#include "error.h"

namespace Mods
{
    /**
     * @return size_t 
     */
    size_t calculate_base64_decoded_length(StringView);

    /**
     * @return size_t 
     */
    size_t calculate_base64_encoded_length(ReadonlyBytes);

    /**
     * @brief only decodes base64
     * 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> decode_base64(StringView);
    
    /**
     * @return String 
     */
    String encode_base64(ReadonlyBytes);

} // namespace Mods

using Mods::decode_base64;
using Mods::encode_base64;