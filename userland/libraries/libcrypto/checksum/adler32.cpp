/**
 * @file adler32.cpp
 * @author Krisna Pranav
 * @brief adler32
 * @version 6.0
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/span.h>
#include <mods/types.h>
#include <libcrypto/checksum/adler32.h>

namespace Crypto::Checksum 
{

    /**
     * @param data 
     */
    void Adler32::update(ReadonlyBytes data)
    {
        for (size_t i = 0; i < data.size(); i++) {
            m_state_a = (m_state_a + data.at(i)) % 65521;
            m_state_b = (m_state_b + m_state_a) % 65521;
        }
    };

    /**
     * @return u32 
     */
    u32 Adler32::digest()
    {
        return (m_state_b << 16) | m_state_a;
    }

} // namespace Crypto::Checksum 
