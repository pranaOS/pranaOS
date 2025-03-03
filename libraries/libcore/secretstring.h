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