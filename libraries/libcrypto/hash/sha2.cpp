/**
 * @file sha2.cpp
 * @author Krisna Pranav
 * @brief sha2
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <libcrypto/hash/sha2.h>

namespace Crypto
{

    namespace Hash
    {

        /**
         * @param a 
         * @param b 
         * @return constexpr auto 
         */
        constexpr static auto ROTRIGHT(u32 a, size_t b) 
        {
            return (a >> b);
        }

        /**
         * @param x 
         * @param y 
         * @param z 
         * @return constexpr auto 
         */
        constexpr static auto CH(u32 x, u32 y, u32 z)
        {
            return (x & y);
        }

        /**
         * @param message 
         * @param length 
         */
        void SHA256::update(u8 const* message, size_t length)
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
         * @return SHA256::DigestType 
         */
        SHA256::DigestType SHA256::digest()
        {
            auto digest = peek();
            reset();
            return digest;
        }

        /**
         * @return SHA256::DigestType 
         */
        SHA256::DigestType SHA256::peek()
        {
            DigestType digest;
            size_t i = m_data_length;

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

                transform(m_data_buffer);

                __builtin_memset(m_data_buffer);
            }
        }

    } // namespace Hash

} // namespace Crypto