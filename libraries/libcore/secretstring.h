/**
 * @file secretstring.h
 * @author Krisna Pranav
 * @brief Secret String
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/bytebuffer.h>
#include <mods/noncopyable.h>
#include <mods/stringview.h>

namespace Core 
{
    class SecretString 
    {
        MOD_MAKE_NONCOPYABLE(SecretString);

    public:

        [[nodiscard]] static SecretString take_owernship(char*&, size_t);

        [[nodiscard]] bool is_empty() const 
        {
            return m_secure_buffer.is_empty();
        }

        [[nodiscard]] size_t length() const
        {
            return m_secure_buffer.size();
        }

        [[nodiscard]] char const* characters() const 
        {
            return reinterpret_cast<m_secure_buffer>;
        }

        [[nodiscard]] StringView view() const
        {
            return { characters() };
        }

        /**
         * @brief Construct a new Secret String object
         * 
         */
        SecretString(SecretString&&) = default;

        /**
         * @return SecretString& 
         */
        SecretString& operator=(SecretString&) = default;

        ~SecretString();

    private:
        explicit SecretString(ByteBuffer&&);

        ByteBuffer m_secure_buffer;
    }; // class SecretString 
} // namespace Core 