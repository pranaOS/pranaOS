/**
 * @file pem.h
 * @author Krisna Pranav
 * @brief PEM
 * @version 6.0
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <libcrypto/asn1/asn1.h>
#include <libcrypto/asn1/der.h>

namespace Crypto 
{
    /**
     * @return ByteBuffer 
     */
    ByteBuffer decode_pem(ReadonlyBytes);
} // namespace Crypto
