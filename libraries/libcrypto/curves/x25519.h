/**
 * @file x25519.h
 * @author Krisna Pranav
 * @brief x25519
 * @version 0.1
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <libcrypto/curves/ellipticcurve.h>

namespace Crypto::Curves 
{

    class X25519 : public EllipticCurve 
    {
    public:
        /**
         * @return size_t 
         */
        size_t key_size() override 
        { 
            return 32; 
        }

        /**
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> generate_private_key() override;

        /**
         * @param a 
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> generate_public_key(ReadonlyBytes a) override;

        /**
         * @param a 
         * @param b 
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> compute_coordinate(ReadonlyBytes a, ReadonlyBytes b) override;

        /**
         * @param shared_point 
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> derive_premaster_key(ReadonlyBytes shared_point) override;
    }; // class X25519 : public EllipticCurve 

} // namespace Crypto::Curves 
