/**
 * @file sha1.cpp
 * @author Krisna Pranav
 * @brief sha1
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/endian.h>
#include <mods/memory.h>
#include <mods/types.h>
#include <libcrypto/hash/sha1.h>

namespace Crypto 
{

    namespace Hash 
    {

        /**
         * @param value 
         * @param bits 
         * @return constexpr auto 
         */
        static constexpr auto ROTATE_LEFT(u32 value, size_t bits)
        {
            return (value << bits) | (value >> (32 - bits));
        }

        /**
         * @param data 
         */
        inline void SHA1::transform(u8 const* data)
        {
            u32 blocks[80];
            for (size_t i = 0; i < 16; ++i)
                blocks[i] = Mods::convert_between_host_and_network_endian(((u32 const*)data)[i]);

            for (size_t i = 16; i < Rounds; ++i)
                blocks[i] = ROTATE_LEFT(blocks[i - 3] ^ blocks[i - 8] ^ blocks[i - 14] ^ blocks[i - 16], 1);

            auto a = m_state[0], b = m_state[1], c = m_state[2], d = m_state[3], e = m_state[4];
            u32 f, k;

            for (size_t i = 0; i < Rounds; ++i) {
                if (i <= 19) {
                    f = (b & c) | ((~b) & d);
                    k = SHA1Constants::RoundConstants[0];
                } else if (i <= 39) {
                    f = b ^ c ^ d;
                    k = SHA1Constants::RoundConstants[1];
                } else if (i <= 59) {
                    f = (b & c) | (b & d) | (c & d);
                    k = SHA1Constants::RoundConstants[2];
                } else {
                    f = b ^ c ^ d;
                    k = SHA1Constants::RoundConstants[3];
                }
                auto temp = ROTATE_LEFT(a, 5) + f + e + k + blocks[i];
                e = d;
                d = c;
                c = ROTATE_LEFT(b, 30);
                b = a;
                a = temp;
            }

            m_state[0] += a;
            m_state[1] += b;
            m_state[2] += c;
            m_state[3] += d;
            m_state[4] += e;

            a = 0;
            b = 0;
            c = 0;
            d = 0;
            e = 0;
            secure_zero(blocks, 16 * sizeof(u32));
        }

        /**
         * @param message 
         * @param length 
         */
        void SHA1::update(u8 const* message, size_t length)
        {
            for (size_t i = 0; i < length; ++i) {
                if (m_data_length == BlockSize) {
                    transform(m_data_buffer);
                    m_bit_length += 512;
                    m_data_length = 0;
                }
                m_data_buffer[m_data_length++] = message[i];
            }
        }

        /**
         * @return SHA1::DigestType 
         */
        SHA1::DigestType SHA1::digest()
        {
            auto digest = peek();
            reset();
            return digest;
        }

        /**
         * @return SHA1::DigestType 
         */
        SHA1::DigestType SHA1::peek()
        {
            DigestType digest;
            size_t i = m_data_length;

            u8 data[BlockSize];
            u32 state[5];
            __builtin_memcpy(data, m_data_buffer, m_data_length);
            __builtin_memcpy(state, m_state, 20);

            if (BlockSize == m_data_length) {
                transform(m_data_buffer);
                m_bit_length += BlockSize * 8;
                m_data_length = 0;
                i = 0;
            }

            if (m_data_length < FinalBlockDataSize) {
                m_data_buffer[i++] = 0x80;
                while (i < FinalBlockDataSize)
                    m_data_buffer[i++] = 0x00;

            } else {
                m_data_buffer[i++] = 0x80;
                while (i < BlockSize)
                    m_data_buffer[i++] = 0x00;
                transform(m_data_buffer);

                __builtin_memset(m_data_buffer, 0, FinalBlockDataSize);
            }

            m_bit_length += m_data_length * 8;
            m_data_buffer[BlockSize - 1] = m_bit_length;
            m_data_buffer[BlockSize - 2] = m_bit_length >> 8;
            m_data_buffer[BlockSize - 3] = m_bit_length >> 16;
            m_data_buffer[BlockSize - 4] = m_bit_length >> 24;
            m_data_buffer[BlockSize - 5] = m_bit_length >> 32;
            m_data_buffer[BlockSize - 6] = m_bit_length >> 40;
            m_data_buffer[BlockSize - 7] = m_bit_length >> 48;
            m_data_buffer[BlockSize - 8] = m_bit_length >> 56;

            transform(m_data_buffer);

            for (size_t i = 0; i < 4; ++i) {
                digest.data[i + 0] = (m_state[0] >> (24 - i * 8)) & 0x000000ff;
                digest.data[i + 4] = (m_state[1] >> (24 - i * 8)) & 0x000000ff;
                digest.data[i + 8] = (m_state[2] >> (24 - i * 8)) & 0x000000ff;
                digest.data[i + 12] = (m_state[3] >> (24 - i * 8)) & 0x000000ff;
                digest.data[i + 16] = (m_state[4] >> (24 - i * 8)) & 0x000000ff;
            }

            __builtin_memcpy(m_data_buffer, data, m_data_length);
            __builtin_memcpy(m_state, state, 20);
            return digest;
        }

    } // namespace Hash

} // namespace Crypto
