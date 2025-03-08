/**
 * @file ellipticcurve.h
 * @author Krisna Pranav
 * @brief elliptic curve
 * @version 6.0
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>

namespace Crypto::Curves 
{

    class EllipticCurve 
    {
    public:
        /**
         * @return size_t 
         */
        virtual size_t key_size() = 0;

        /**
         * @return ErrorOr<ByteBuffer> 
         */
        virtual ErrorOr<ByteBuffer> generate_private_key() = 0;

        /**
         * @param a 
         * @return ErrorOr<ByteBuffer> 
         */
        virtual ErrorOr<ByteBuffer> generate_public_key(ReadonlyBytes a) = 0;

        /**
         * @param scalar_bytes 
         * @param point_bytes 
         * @return ErrorOr<ByteBuffer> 
         */
        virtual ErrorOr<ByteBuffer> compute_coordinate(ReadonlyBytes scalar_bytes, ReadonlyBytes point_bytes) = 0;

        /**
         * @param shared_point 
         * @return ErrorOr<ByteBuffer> 
         */
        virtual ErrorOr<ByteBuffer> derive_premaster_key(ReadonlyBytes shared_point) = 0;

        /**
         * @brief Destroy the Elliptic Curve object
         * 
         */
        virtual ~EllipticCurve() = default;
    }; // class EllipticCurve

} // namespace Crypto::Curves
