/**
 * @file badge.h
 * @author Krisna Pranav
 * @brief Badge
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods {

    template<typename T>
    class Badge {
        friend T;
        Badge() { }
        
        /**
         * @brief Construct a new Badge object
         * 
         */
        Badge(const Badge&) = delete;

        /**
         * @return Badge& 
         */
        Badge& operator=(const Badge&) = delete;

        /**
         * @brief Construct a new Badge object
         * 
         */
        Badge(Badge&&) = delete;

        /**
         * @return Badge& 
         */
        Badge& operator=(Badge&&) = delete;
    };

}

using Mods::Badge;
