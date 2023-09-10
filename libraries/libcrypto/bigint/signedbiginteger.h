/**
 * @file signedbiginteger.h
 * @author Krisna Pranav
 * @brief signed big integer
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "unsignedbiginteger.h"
#include <mods/span.h>
#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto
{
    struct SignedDivisionResult;

    class SignedBigInteger
    {
    public:
        /**
         * @param x 
         */
        SignedBigInteger(i32 x)
            : m_sign(x < 0)
            , m_unsigned_data(abs(x))
        {}

        /**
         * @param unsigned_data 
         * @param sign 
         */
        SignedBigInteger(UnsignedBigInteger&& unsigned_data, bool sign)
            : m_sign(sign)
            , m_unsigned_data(move(unsigned_data))
        {}

        /**
         * @param unsigned_data 
         */
        explicit SignedBigInteger(UnsignedBigInteger unsigned_data)
            : m_sign(false)
            , m_unsinged_data()
        {}

        /**
         * @return SignedBigInteger 
         */
        static SignedBigInteger create_invalid()    
        {
            return {UnsignedBigInteger::create_invalid(), false};
        }

    private:
        bool m_sign { false };
        UnsignedBigInteger m_unsigned_data;
    }; // class SignedBigInteger;

    struct SignedDivisionResult 
    {
        Crypto::SignedBigInteger quotient;
        Crypto::SignedBigInteger remainder;
    };

} // namespace Crypto
