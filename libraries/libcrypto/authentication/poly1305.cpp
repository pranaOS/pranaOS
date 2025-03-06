/**
 * @file poly1305.cpp
 * @author Krisna Pranav
 * @brief poly1305
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/endian.h>
#include <libcrypto/authentication/poly1305.h>

namespace Crypto::Authentication 
{

    /**
     * @brief Construct a new Poly 1305::Poly1305 object
     * 
     * @param key 
     */
    Poly1305::Poly1305(ReadonlyBytes key)
    {
        for (size_t i = 0; i < 16; i += 4) {
            m_state.r[i / 4] = Mods::convert_between_host_and_little_endian(ByteReader::load32(key.offset(i)));
        }

        m_state.r[0] &= 0x0FFFFFFF;
        m_state.r[1] &= 0x0FFFFFFC;
        m_state.r[2] &= 0x0FFFFFFC;
        m_state.r[3] &= 0x0FFFFFFC;

        for (size_t i = 16; i < 32; i += 4) {
            m_state.s[(i - 16) / 4] = Mods::convert_between_host_and_little_endian(ByteReader::load32(key.offset(i)));
        }
    }


} // namespace Crypto::Authentication 
