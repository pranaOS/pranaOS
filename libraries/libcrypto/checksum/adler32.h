/**
 * @file adler32.h
 * @author Krisna Pranav
 * @brief adler32
 * @version 6.0
 * @date 2025-03-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/types.h>
#include <libcrypto/checksum/checksumfunction.h>

namespace Crypto::Checksum 
{

    class Adler32 : public ChecksumFunction<u32> 
    {
    public:
        /**
         * @brief Construct a new Adler32 object
         * 
         */
        Adler32() = default;

        /**
         * @brief Construct a new Adler32 object
         * 
         * @param data 
         */
        Adler32(ReadonlyBytes data)
        {
            update(data);
        }

        /**
         * @brief Construct a new Adler32 object
         * 
         * @param initial_a 
         * @param initial_b 
         * @param data 
         */
        Adler32(u32 initial_a, u32 initial_b, ReadonlyBytes data)
            : m_state_a(initial_a)
            , m_state_b(initial_b)
        {
            update(data);
        }

        /**
         * @param data 
         */
        virtual void update(ReadonlyBytes data) override;

        /**
         * @return u32 
         */
        virtual u32 digest() override;

    private:
        u32 m_state_a { 1 };
        u32 m_state_b { 0 };
    }; // class Adler32 : public ChecksumFunction<u32> 

} // namespace Crypto::Checksum 
