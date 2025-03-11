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
        auto buffer = ByteBuffer::copy(cstring, length).release_value_but_fixme_should_propagate_errors();

        secure_zero(cstring, length);
        free(cstring);

        cstring = nullptr;

        return SecretString(move(buffer));
    }

    /**
     * @param buffer 
     * @return SecretString 
     */
    SecretString SecretString::take_ownership(ByteBuffer&& buffer)
    {
        return SecretString(move(buffer));
    }

    /**
     * @brief Construct a new SecretString::SecretString object
     * 
     * @param buffer 
     */
    SecretString::SecretString(ByteBuffer&& buffer)
        : m_secure_buffer(move(buffer))
    {
        if (m_secure_buffer.is_empty() || (m_secure_buffer[m_secure_buffer.size() - 1] != 0)) {
            u8 nul = '\0';
            m_secure_buffer.append(&nul, 1);
        }
    }

    /**
     * @brief Destroy the SecretString::SecretString object
     * 
     */
    SecretString::~SecretString()
    {
        if (!m_secure_buffer.is_empty()) {
            secure_zero(m_secure_buffer.data(), m_secure_buffer.capacity());
        }
    }

} // namespace Core
