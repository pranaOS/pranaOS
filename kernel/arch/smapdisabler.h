/**
 * @file smapdisabler.h
 * @author Krisna Pranav
 * @brief Smap Disabler
 * @version 6.0
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>

namespace Kernel 
{

    class SmapDisabler 
    {
    public:
        /**
         * @brief Construct a new Smap Disabler object
         * 
         */
        SmapDisabler();

        /**
         * @brief Destroy the Smap Disabler object
         * 
         */
        ~SmapDisabler();

    private:
        const FlatPtr m_flags;
    }; // class SmapDisabler

} // namespace Kernel
