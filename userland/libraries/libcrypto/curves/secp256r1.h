/**
 * @file secp256r1.h
 * @author Krisna Pranav
 * @brief secp256r1
 * @version 6.0
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/ufixedbigint.h>
#include <libcrypto/curves/ellipticcurve.h>

namespace Crypto::Curves 
{

    struct JacobianPoint {
        u256 x { 0u };
        u256 y { 0u };
        u256 z { 0u };
    }; // struct JacobianPoint

    class SECP256r1 : public EllipticCurve 
    {
    public:
        /**
         * @return size_t 
         */
        size_t key_size() override 
        { 
            return 1 + 2 * 32; 
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
         * @param scalar_bytes 
         * @param point_bytes 
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> compute_coordinate(ReadonlyBytes scalar_bytes, ReadonlyBytes point_bytes) override;

        /**
         * @param shared_point 
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> derive_premaster_key(ReadonlyBytes shared_point) override;
    }; // class SECP256r1 : public EllipticCurve 

} // namespace Crypto::Curves 
