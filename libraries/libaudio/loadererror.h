/**
 * @file loader error.h
 * @author Krisna Pranav
 * @brief Loader Error
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/flystring.h>
#include <errno.h>

namespace Audio
{
    struct LoaderError
    {
        enum class Category : u32
        {
            Unknown,
            IO,
            Format,
            Internal,
            Unimplemented
        }; // enum class Category : u32

        Category category { Category::Unknown };
        size_t index { 0 };
        FlyString description { String::empty() };

        constexpr LoaderError() = default;

        /**
         * @brief Construct a new LoaderError object
         * 
         * @param category 
         * @param index 
         * @param description 
         */
        LoaderError(Category category, size_t index, FlyString description)
            : category(category)
            , index(index)
            , description(move(description))
        {}

        /**
         * @brief Construct a new LoaderError object
         * 
         * @param description 
         */
        LoaderError(FlyString description)
            : description(move(description))
        {}
    }; // struct LoaderError
} // namespace Audio