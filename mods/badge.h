/**
 * @file badge.h
 * @author Krisna Pranav
 * @brief Badge
 * @version 1.0
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
        Badge() {};

        Badge(const Badge&) = default;
        Badge& operator=(const Badge&) = default;

        Badge(Badge&&) = default;
    };
}

using Mods::Badge;