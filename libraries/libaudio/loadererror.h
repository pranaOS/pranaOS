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

namespace Audio {

    struct LoaderError 
    {

        enum class Category : u32 
        {
            Unknown = 0,
            IO,
            Format,
            Internal,
            Unimplemented,
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
        {
        }

        /**
         * @brief Construct a new LoaderError object
         * 
         * @param description 
         */
        LoaderError(FlyString description)
            : description(move(description))
        {
        }

        /**
         * @brief Construct a new LoaderError object
         * 
         * @param category 
         * @param description 
         */
        LoaderError(Category category, FlyString description)
            : category(category)
            , description(move(description))
        {
        }

        /**
         * @brief Construct a new LoaderError object
         * 
         */
        LoaderError(LoaderError&) = default;

        /**
         * @brief Construct a new LoaderError object
         * 
         */
        LoaderError(LoaderError&&) = default;

        /**
         * @brief Construct a new LoaderError object
         * 
         * @param error 
         */
        LoaderError(Error&& error)
        {
            if (error.is_errno()) {
                auto code = error.code();
                description = String::formatted("{} ({})", strerror(code), code);
                if (code == EBADF || code == EBUSY || code == EEXIST || code == EIO || code == EISDIR || code == ENOENT || code == ENOMEM || code == EPIPE)
                    category = Category::IO;
            } else {
                description = error.string_literal();
            }
        }
    }; // struct LoaderError

} // namespace Audio

#define LOADER_TRY(expression)                                     \
    ({                                                             \
        auto _temporary_result = (expression);                     \
        if (_temporary_result.is_error())                          \
            return LoaderError(_temporary_result.release_error()); \
        _temporary_result.release_value();                         \
    })
