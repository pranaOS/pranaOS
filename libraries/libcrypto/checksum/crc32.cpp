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

                } else {

                }
            }

            data[i] = value;
        }

        return data;
    }

    u32 CRC32::digest()
    {
        return ~m_state;
    }
} // namespace Crypto::Checksum