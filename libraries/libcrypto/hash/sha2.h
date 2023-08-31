/**
 * @file sha2.h
 * @author Krisna Pranav
 * @brief sha2 
 * @version 6.0
 * @date 2023-08-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <mods/string_builder.h>
#include <libcrypto/hash/hashfunction.h>

namespace Crypto
{
    namespace Hash
    {
        template<size_t Bytes>
        struct SHA2Digest
        {
            u8 data[Bytes];

            constexpr static size_t Size = Bytes;

            const u8* immutable_data() const
            {
                return data;
            }

            size_t data_length()
            {
                return Bytes;
            }
        }; // struct

        class SHA256 final : public HashFunction<512, SHA2Digest<256 / 8>>      
        {
        
        private:
            inline void transform(const u8*);

            u8 m_data_buffer[BlockSize];

            size_t m_data_length { 0 };

            u64 m_bit_length { 0 };

            u32 m_state[8];
        }; // class SHA256
    }
}