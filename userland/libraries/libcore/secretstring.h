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
        /**
         * @return SecretString 
         */
        [[nodiscard]] static SecretString take_ownership(char*&, size_t);

        /**
         * @return SecretString 
         */
        [[nodiscard]] static SecretString take_ownership(ByteBuffer&&);

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const 
        { 
            return m_secure_buffer.is_empty(); 
        }

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t length() const 
        { 
            return m_secure_buffer.size(); 
        }

        /**
         * @return char const* 
         */
        [[nodiscard]] char const* characters() const 
        { 
            return reinterpret_cast<char const*>(m_secure_buffer.data()); 
        }

        /**
         * @return StringView 
         */
        [[nodiscard]] StringView view() const 
        { 
            return { characters(), length() }; 
        }

        /**
         * @brief Construct a new SecretString object
         * 
         */
        SecretString() = default;

        /**
         * @brief Destroy the SecretString object
         * 
         */
        ~SecretString();

        /**
         * @brief Construct a new SecretString object
         * 
         */
        SecretString(SecretString&&) = default;

        /**
         * @return SecretString& 
         */
        SecretString& operator=(SecretString&&) = default;

    private:
        /**
         * @brief Construct a new SecretString object
         * 
         */
        explicit SecretString(ByteBuffer&&);

        ByteBuffer m_secure_buffer;
    }; // class SecretString

} // namespace Core
