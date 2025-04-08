/**
 * @file fly_string.h
 * @author Krisna Pranav
 * @brief fly string
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/error.h>
#include <mods/format.h>
#include <mods/platform.h>
#include <mods/string.h>
#include <mods/traits.h>
#include <mods/types.h>

namespace Mods
{

    class FlyString
    {
        MOD_MAKE_DEFAULT_MOVABLE(FlyString);
        MOD_MAKE_DEFAULT_COPYABLE(FlyString);

    public:
        /**
         * @brief Construct a new Fly String object
         * 
         */
        FlyString() = default;

        static ErrorOr<FlyString> from_utf8(StringView);
        static FlyString from_utf8_without_validation(ReadonlyBytes);

        /**
         * @tparam T 
         */
        template <typename T>
            requires(IsOneOf<RemoveCVReference<T>, ByteString, DeprecatedFlyString, FlyString, String>)
        static ErrorOr<String> from_utf8(T&&) = delete;

        /**
         * @brief Construct a new Fly String object
         * 
         */
        FlyString(String const&);

        /**
         * @return FlyString& 
         */
        FlyString& operator=(String const&);

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] unsigned hash() const;

        /**
         * @return u32 
         */
        [[nodiscard]] u32 ascii_case_insensitive_hash() const;

        explicit operator String() const;

        /**
         * @return String 
         */
        String to_string() const;

        /**
         * @return Utf8View 
         */
        [[nodiscard]] Utf8View code_points() const;

        /**
         * @return ReadonlyBytes 
         */
        [[nodiscard]] ReadonlyBytes bytes() const;

        /**
         * @return StringView 
         */
        [[nodiscard]] StringView bytes_as_string_view() const;

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool operator==(FlyString const& other) const
        {
            return m_data.raw({}) == other.m_data.raw({});
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool operator==(String const&) const;
        [[nodiscard]] bool operator==(StringView) const;
        [[nodiscard]] bool operator==(char const*) const;

        /**
         * @param other 
         */
        [[nodiscard]] int operator<=>(FlyString const& other) const;

        static void did_destroy_fly_string_data(Badge<Detail::StringData>, Detail::StringData const&);

        [[nodiscard]] Detail::StringBase data(Badge<String>) const;

        [[nodiscard]] static size_t number_of_fly_strings();

        [[nodiscard]] DeprecatedFlyString to_deprecated_fly_string() const;
        static ErrorOr<FlyString> from_deprecated_fly_string(DeprecatedFlyString const&);

        /**
         * @tparam T 
         */
        template <typename T>
            requires(IsSame<RemoveCVReference<T>, StringView>)
        static ErrorOr<String> from_deprecated_fly_string(T&&) = delete;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool equals_ignoring_ascii_case(FlyString const&) const;
        [[nodiscard]] bool equals_ignoring_ascii_case(StringView) const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool starts_with_bytes(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool ends_with_bytes(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @tparam Ts 
         * @param strings 
         * @return ALWAYS_INLINE constexpr 
         */
        template <typename... Ts>
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_one_of(Ts... strings) const
        {
            return (... || this->operator==(forward<Ts>(strings)));
        }

    private:
        /**
         * @brief Construct a new Fly String object
         * 
         * @param data 
         */
        explicit FlyString(Detail::StringBase data)
            : m_data(move(data))
        {
        }

        Detail::StringBase m_data;
    }; // class FlyString

    /**
     * @tparam  
     */
    template <>
    struct Traits<FlyString> : public DefaultTraits<FlyString>
    {
        static unsigned hash(FlyString const&);
    };

    /**
     * @tparam  
     */
    template <>
    struct Formatter<FlyString> : Formatter<StringView>
    {
        ErrorOr<void> format(FormatBuilder&, FlyString const&);
    };

    struct ASCIICaseInsensitiveFlyStringTraits : public Traits<String>
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(FlyString const& s)
        {
            return s.ascii_case_insensitive_hash();
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(FlyString const& a, FlyString const& b)
        {
            return a.equals_ignoring_ascii_case(b);
        }
    }; // struct ASCIICaseInsensitiveFlyStringTraits : public Traits<String>

} // namespace Mods

/**
 * @param cstring 
 * @param length 
 * @return ALWAYS_INLINE 
 */
[[nodiscard]] ALWAYS_INLINE Mods::FlyString operator""_fly_string(char const* cstring, size_t length)
{
    return Mods::FlyString::from_utf8(Mods::StringView(cstring, length)).release_value();
}

#if USING_MODS_GLOBALLY
using Mods::FlyString;
#endif
