/**
 * @file badge.h
 * @author Krisna Pranav
 * @brief Badge
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods 
{
    /**
     * @tparam T 
     */
    template<typename T>
    class Badge 
    {
    public:
        using Type = T;

    private:
        friend T;

        constexpr Badge() = default;

        /**
         * @brief Construct a new Badge object
         * 
         */
        Badge(Badge const&) = delete;

        /**
         * @return Badge& 
         */
        Badge& operator=(Badge const&) = delete;

        /**
         * @brief Construct a new Badge object
         * 
         */
        Badge(Badge&&) = delete;

        /**
         * @return Badge& 
         */
        Badge& operator=(Badge&&) = delete;
    }; // class Badge

} // namespace Mods

using Mods::Badge;