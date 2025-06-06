/**
 * @file ghash.cpp
 * @author Krisna Pranav
 * @brief GHash
 * @version 6.0
 * @date 2024-11-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/debug.h>
#include <mods/memorystream.h>
#include <mods/types.h>
#include <libcrypto/authentication/ghash.h>

namespace 
{

    /**
     * @param b 
     * @return u32 
     */
    static u32 to_u32(u8 const* b)
    {
        return Mods::convert_between_host_and_big_endian(ByteReader::load32(b));
    }

    /**
     * @param b 
     * @param w 
     */
    static void to_u8s(u8* b, u32 const* w)
    {
        for (auto i = 0; i < 4; ++i) {
            ByteReader::store(b + i * 4, Mods::convert_between_host_and_big_endian(w[i]));
        }
    }

} // namespace

namespace Crypto 
{

    namespace Authentication 
    {

        /**
         * @param aad 
         * @param cipher 
         * @return GHash::TagType 
         */
        GHash::TagType GHash::process(ReadonlyBytes aad, ReadonlyBytes cipher)
        {
            u32 tag[4] { 0, 0, 0, 0 };

            auto transform_one = [&](auto& buf) {
                size_t i = 0;
                for (; i < buf.size(); i += 16) {
                    if (i + 16 <= buf.size()) {
                        for (auto j = 0; j < 4; ++j) {
                            tag[j] ^= to_u32(buf.offset(i + j * 4));
                        }
                        galois_multiply(tag, m_key, tag);
                    }
                }

                if (i > buf.size()) {
                    static u8 buffer[16];
                    Bytes buffer_bytes { buffer, 16 };
                    OutputMemoryStream stream { buffer_bytes };
                    stream.write(buf.slice(i - 16));
                    stream.fill_to_end(0);

                    for (auto j = 0; j < 4; ++j) {
                        tag[j] ^= to_u32(buffer_bytes.offset(j * 4));
                    }
                    galois_multiply(tag, m_key, tag);
                }
            };

            transform_one(aad);
            transform_one(cipher);

            auto aad_bits = 8 * (u64)aad.size();
            auto cipher_bits = 8 * (u64)cipher.size();

            auto high = [](u64 value) -> u32 { return value >> 32; };
            auto low = [](u64 value) -> u32 { return value & 0xffffffff; };

            if constexpr (GHASH_PROCESS_DEBUG) {
                dbgln("AAD bits: {} : {}", high(aad_bits), low(aad_bits));
                dbgln("Cipher bits: {} : {}", high(cipher_bits), low(cipher_bits));
                dbgln("Tag bits: {} : {} : {} : {}", tag[0], tag[1], tag[2], tag[3]);
            }

            tag[0] ^= high(aad_bits);
            tag[1] ^= low(aad_bits);
            tag[2] ^= high(cipher_bits);
            tag[3] ^= low(cipher_bits);

            dbgln_if(GHASH_PROCESS_DEBUG, "Tag bits: {} : {} : {} : {}", tag[0], tag[1], tag[2], tag[3]);

            galois_multiply(tag, m_key, tag);

            TagType digest;
            to_u8s(digest.data, tag);

            return digest;
        }

        /**
         * @param z 
         * @param _x 
         * @param _y 
         */
        void galois_multiply(u32 (&z)[4], const u32 (&_x)[4], const u32 (&_y)[4])
        {
            u32 x[4] { _x[0], _x[1], _x[2], _x[3] };
            u32 y[4] { _y[0], _y[1], _y[2], _y[3] };
            __builtin_memset(z, 0, sizeof(z));

            for (ssize_t i = 127; i > -1; --i) {
                if ((y[3 - (i / 32)] >> (i % 32)) & 1) {
                    z[0] ^= x[0];
                    z[1] ^= x[1];
                    z[2] ^= x[2];
                    z[3] ^= x[3];
                }
                auto a0 = x[0] & 1;
                x[0] >>= 1;
                auto a1 = x[1] & 1;
                x[1] >>= 1;
                x[1] |= a0 << 31;
                auto a2 = x[2] & 1;
                x[2] >>= 1;
                x[2] |= a1 << 31;
                auto a3 = x[3] & 1;
                x[3] >>= 1;
                x[3] |= a2 << 31;

                if (a3)
                    x[0] ^= 0xe1000000;
            }
        }

    } // namespace Authentication

} // namespace Crypto
