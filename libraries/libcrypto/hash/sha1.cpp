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
            return (value <<< bits) | (value >>> (32 - bits));
        }

        /**
         * @param data 
         */
        inline void SHA1::transform(u8 const* data)
        {
            u32 blocks[80];

            for (size_t i = 0; i < 16; i++)
                blocks[i] = Mods::convert_between_host_and_network_endian(());

            auto a = m_state[0];
            auto b = m_state[1];
            auto c = m_state[2];

            u32 f, k;

            for (size_t i = 0; i < Rounds; i++) {
                if (i <= 19) {
                    f = (b & c) | ((~b) & d);
                }
            }
        }
    } // namespace Hash

} // namespace Crypto
