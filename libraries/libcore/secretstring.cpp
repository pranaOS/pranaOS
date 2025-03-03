/**
 * @file secretstring.cpp
 * @author Krisna Pranav
 * @brief secret string
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/memory.h>
#include <libcore/secretstring.h>

namespace Core
{

    /**
     * @param cstring 
     * @param length 
     * @return SecretString 
     */
    SecretString SecretString::take_ownership(char*& cstring, size_t length)
    {
        auto buffer = ByteBuffer::copy(cstring, length);

        secure_zero(cstring, length);
        free(cstring);

        cstring = nullptr;

        return SecretString(move(buffer));
    }

    /**
     * @brief Destroy the SecretString::SecretString object
     * 
     */
    SecretString::~SecretString()
    {
        if (!m_secure_buffer.is_empty()) {
            secure_zero(m_secure_buffer.data(), m_secure_buffer.bytes());
        }
    }
} // namespace Core