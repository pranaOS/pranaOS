/**
 * @file ghash.h
 * @author Krisna Pranav
 * @brief ghash
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytereader.h>
#include <mods/endian.h>
#include <mods/types.h>
#include <libcrypto/hash/hashfunction.h>

#ifndef KERNEL
#    include <mods/string.h>
#endif

namespace Crypto 
{
    namespace Authentication 
    {
        /**
         * @param z 
         * @param x 
         * @param y 
         */
        void galois_multiply(u32 (&z)[4], const u32 (&x)[4], const u32 (&y)[4]);

        struct GHashDigest 
        {
            constexpr static size_t Size = 16;
            u8 data[Size];

            const u8* immutable_data() const { return data; }
            size_t data_length() { return Size; }
        }; // struct GHashDigest 

        class GHash final {
        public:
            using TagType = GHashDigest;

            template<size_t N>
            explicit GHash(const char (&key)[N])
                : GHash({ key, N })
            {
            }

            explicit GHash(ReadonlyBytes key)
            {
                VERIFY(key.size() >= 16);
                for (size_t i = 0; i < 16; i += 4) {
                    m_key[i / 4] = Mods::convert_between_host_and_big_endian(ByteReader::load32(key.offset(i)));
                }
            }

            constexpr static size_t digest_size() { return TagType::Size; }

        #ifndef KERNEL
            String class_name() const
            {
                return "GHash";
            }
        #endif

            TagType process(ReadonlyBytes aad, ReadonlyBytes cipher);

        private:
            inline void transform(ReadonlyBytes, ReadonlyBytes);

            u32 m_key[4];
        }; // class GHash final
    } // namespace Authentication
} // namespace Crypto 