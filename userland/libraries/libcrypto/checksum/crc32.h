/**
 * @file crc32.h
 * @author Krisna Pranav
 * @brief CRC 32
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

    class CRC32 : public ChecksumFunction<u32> 
    {
    public:
        /**
         * @brief Construct a new CRC32 object
         * 
         */
        CRC32() = default;

        /**
         * @brief Construct a new CRC32 object
         * 
         * @param data 
         */
        CRC32(ReadonlyBytes data)
        {
            update(data);
        }

        /**
         * @brief Construct a new CRC32 object
         * 
         * @param initial_state 
         * @param data 
         */
        CRC32(u32 initial_state, ReadonlyBytes data)
            : m_state(initial_state)
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
        u32 m_state { ~0u };
    }; // class CRC32 : public ChecksumFunction<u32> 

} // namespace Crypto::Checksum
