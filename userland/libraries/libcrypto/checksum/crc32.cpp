/**
 * @file crc32.cpp
 * @author Krisna Pranav
 * @brief crc32
 * @version 6.0
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/array.h>
#include <mods/span.h>
#include <mods/types.h>
#include <libcrypto/checksum/crc32.h>

namespace Crypto::Checksum 
{

    static constexpr auto generate_table()
    {
        Array<u32, 256> data {};

        for (auto i = 0u; i < data.size(); i++) {
            u32 value = i;

            for (auto j = 0; j < 8; j++) {
                if (value & 1) {
                    value = 0xEDB88320 ^ (value >> 1);
                } else {
                    value = value >> 1;
                }
            }

            data[i] = value;
        }
        return data;
    }

    static constexpr auto table = generate_table();

    /**
     * @param data 
     */
    void CRC32::update(ReadonlyBytes data)
    {
        for (size_t i = 0; i < data.size(); i++) {
            m_state = table[(m_state ^ data.at(i)) & 0xFF] ^ (m_state >> 8);
        }
    };

    /**
     * @return u32 
     */
    u32 CRC32::digest()
    {
        return ~m_state;
    }

} // namespace Crypto::Checksum
