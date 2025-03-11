/**
 * @file checksumfunction.h
 * @author Krisna Pranav
 * @brief check sum function
 * @version 6.0
 * @date 2025-03-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>

namespace Crypto::Checksum 
{

    /**
     * @tparam ChecksumType 
     */
    template<typename ChecksumType>
    class ChecksumFunction 
    {
    public: 
        /**
         * @param data 
         */
        virtual void update(ReadonlyBytes data) = 0;

        /**
         * @return ChecksumType 
         */
        virtual ChecksumType digest() = 0;

    protected:
        /**
         * @brief Destroy the ChecksumFunction object
         * 
         */
        virtual ~ChecksumFunction() = default;
    }; // class class ChecksumFunction

} // namespace Crypto::Checksum 
