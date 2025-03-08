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

    struct JacobainPoint
    {

    }; // struct JacobainPoint

    class SECP256r1 : public EllipticCurve
    {
    public:
    
        size_t key_size() override
        {
            return 1 + 2 * 32;
        }

    }; // class SECP256r1 : public EllipticCurve

} // namespace Crypto::Curves
