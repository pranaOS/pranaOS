/**
 * @file uuid.h
 * @author Krisna Pranav
 * @brief UUID
 * @version 6.0
 * @date 2025-02-16
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/array.h>
#include <mods/bytebuffer.h>
#include <mods/stringview.h>
#include <mods/types.h>

#ifdef KERNEL
#include <kernel/kstring.h>
#else
#include <mods/string.h>
#endif

namespace Mods
{
    class UUID
    {
    public:
        enum class Endianness
        {
            Mixed,
            Little
        }; // enum class Endianness

        /**
         * @brief Construct a new UUID object
         * 
         */
        UUID() = default;

        /**
         * @brief Construct a new UUID object
         * 
         * @param uuid_buffer 
         */
        UUID(Array<u8, 16> uuid_buffer);

        /**
         * @brief Construct a new UUID object
         * 
         * @param endianness 
         */
        UUID(StringView, Endianness endianness = Endianness::Little);

        /**
         * @brief Destroy the UUID object
         * 
         */
        ~UUID() = default;

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const UUID&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const UUID& other) const
        {
            return !(*this == other);
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator<=(const UUID&) const = delete;
        bool operator>=(const UUID&) const = delete;
        bool operator<(const UUID&) const = delete;
        bool operator>(const UUID&) const = delete;

    #ifdef KERNEL
        ErrorOr<NonnullOwnPtr<Kernel::KString>> to_string() const;
    #else
        String to_string() const;
    #endif
        bool is_zero() const;

    private:
        void convert_string_view_to_little_endian_uuid(StringView);
        void convert_string_view_to_mixed_endian_uuid(StringView);

        Array<u8, 16> m_uuid_buffer{};
    }; // class UUID
} // namespace Mods

using Mods::UUID;
