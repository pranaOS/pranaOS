/**
 * @file networkresponse.h
 * @author Krisna Pranav
 * @brief network response
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/refcounted.h>

namespace Core 
{

    class NetworkResponse : public RefCounted<NetworkResponse> 
    {
    public:
        /**
         * @brief Destroy the NetworkResponse object
         * 
         */
        virtual ~NetworkResponse() = default;

        /**
         * @return true 
         * @return false 
         */
        bool is_error() const 
        { 
            return m_error; 
        }

    protected:
        explicit NetworkResponse() = default;

        bool m_error { false };
    }; // class NetworkResponse : public RefCounted<NetworkResponse>
 
} // namespace Core
