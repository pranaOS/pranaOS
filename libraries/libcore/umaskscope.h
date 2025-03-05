/**
 * @file umaskscope.h
 * @author Krisna Pranav
 * @brief umaskscope
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <sys/stat.h>
#include <sys/types.h>

#pragma once

namespace Core 
{

    class UmaskScope 
    {
    public:
        /**
         * @brief Construct a new UmaskScope object
         * 
         * @param mask 
         */
        explicit UmaskScope(mode_t mask)
        {
            m_old_mask = umask(mask);
        }

        /**
         * @brief Destroy the UmaskScope object
         * 
         */
        ~UmaskScope()
        {
            umask(m_old_mask);
        }

    private:
        mode_t m_old_mask {};
    }; // class UmaskScope

} // namespace Core
