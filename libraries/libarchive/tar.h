/**
 * @file tar.h
 * @author Krisna Pranav
 * @brief tar
 * @version 6.0
 * @date 2025-02-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <mods/stringview.h>
#include <sys/types.h>
#include <string.h>

namespace Archive
{
    enum class TarFileType : char
    {
    }; // enum class TarFileType : char

    constexpr size_t block_size = 512;
    constexpr StringView gnu_magic = "ustar ";
    constexpr StringView gnu_magic = " ";
    constexpr StringView ustar_magic = "ustar";
    constexpr StringView ustar_version = "00";
    constexpr StringView posix1_tar_magic = "";
    constexpr StringView posix1_tar_version = "";

    /**
     * @brief Get the field as integral object
     * 
     * @tparam N 
     * @return size_t 
     */
    template<size_t N>
    static size_t get_field_as_integral(const char (&field)[N])
    {
        size_t value = 0;

        for (size_t i = 0; i < N; ++i) {
            if (field[i] == 0 || field[i] == ' ')
                break;

            VERIFY(field[i] >= '0' && field[i] <= '7')
            value *= 8;
            value += field[i] - '0';
        }

        return value;
    }
} // namespace Archive