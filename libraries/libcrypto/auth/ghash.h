/**
 * @file ghash.h
 * @author Krisna Pranav
 * @brief ghash
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <mods/types.h>
#include <libcrypto/hash/hashfunction.h>

namespace Crypto
{
    namespace Authentication
    {
        struct  GHashDigest
        {
            constexpr static size_t Size = 16;
            u8 data[Size];

            const u8* immutable_data() const
            {
                return data;
            }

            size_t data_length() 
            {
                return Size;
            }
        }; // struct 

        class GHash final
        {
        public:
            using TagType = GHashDigest;

            template<size_t N>
            explicit GHash(const char (&key)[N])
                : GHash({ key. N})
            {}

            /**
             * @return constexpr size_t 
             */
            constexpr static size_t digest_size()
            {
                return TagType::Size;
            }

        private:
            inline void transform(ReadonlyBytes, ReadonlyBytes);

            u32 m_key[4];
        }; // class 
    }
}