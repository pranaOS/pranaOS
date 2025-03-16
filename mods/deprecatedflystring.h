/**
 * @file deprecatedflystring.h
 * @author Krisna Pranav
 * @brief deprecated fly string
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytestring.h>
#include <mods/stringutils.h>

namespace Mods 
{

    class DeprecatedFlyString 
    {
    public:
        /**
         * @brief Construct a new DeprecatedFlyString object
         * 
         */
        DeprecatedFlyString()
            : m_impl(StringImpl::the_empty_stringimpl())
        {
        }

        /**
         * @brief Construct a new DeprecatedFlyString object
         * 
         * @param other 
         */
        DeprecatedFlyString(DeprecatedFlyString const& other)
            : m_impl(other.impl())
        {
        }

        /**
         * @brief Construct a new DeprecatedFlyString object
         * 
         * @param other 
         */
        DeprecatedFlyString(DeprecatedFlyString&& other)
            : m_impl(move(other.m_impl))
        {
        }

        /**
         * @brief Construct a new DeprecatedFlyString object
         * 
         */
        DeprecatedFlyString(ByteString const&);
        DeprecatedFlyString(StringView);

        /**
         * @brief Construct a new DeprecatedFlyString object
         * 
         * @param string 
         */
        DeprecatedFlyString(char const* string)
            : DeprecatedFlyString(static_cast<ByteString>(string))
        {
        }

        /**
         * @param impl 
         * @return DeprecatedFlyString 
         */
        static DeprecatedFlyString from_fly_impl(NonnullRefPtr<StringImpl const> impl)
        {
            VERIFY(impl->is_fly());
            DeprecatedFlyString string;
            string.m_impl = move(impl);
            return string;
        }

        /**
         * @param other 
         * @return DeprecatedFlyString& 
         */
        DeprecatedFlyString& operator=(DeprecatedFlyString const& other)
        {
            m_impl = other.m_impl;
            return *this;
        }

        /**
         * @param other 
         * @return DeprecatedFlyString& 
         */
        DeprecatedFlyString& operator=(DeprecatedFlyString&& other)
        {
            m_impl = move(other.m_impl);
            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return !m_impl->length(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(DeprecatedFlyString const& other) const 
        { 
            return m_impl == other.m_impl; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(ByteString const&) const;

        bool operator==(StringView) const;

        bool operator==(char const*) const;

        /**
         * @return NonnullRefPtr<StringImpl const> 
         */
        NonnullRefPtr<StringImpl const> impl() const 
        { 
            return m_impl; 
        }

        /**
         * @return char const* 
         */
        char const* characters() const 
        { 
            return m_impl->characters(); 
        }

        /**
         * @return size_t 
         */
        size_t length() const 
        { 
            return m_impl->length(); 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 hash() const 
        { 
            return m_impl->existing_hash(); 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE StringView view() const 
        { 
            return m_impl->view(); 
        }

        /**
         * @return DeprecatedFlyString 
         */
        DeprecatedFlyString to_lowercase() const;

        /**
         * @tparam T 
         * @param trim_whitespace 
         * @return Optional<T> 
         */
        template<Arithmetic T>
        Optional<T> to_number(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const
        {
            return view().to_number<T>(trim_whitespace);
        }

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_ascii_case(StringView) const;

        /**
         * @return true 
         * @return false 
         */
        bool starts_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @return true 
         * @return false 
         */
        bool ends_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        static void did_destroy_impl(Badge<StringImpl>, StringImpl&);

        /**
         * @tparam Ts 
         * @param strings 
         * @return ALWAYS_INLINE constexpr 
         */
        template<typename... Ts>
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_one_of(Ts... strings) const
        {
            return (... || this->operator==(forward<Ts>(strings)));
        }

    private:
        NonnullRefPtr<StringImpl const> m_impl;
    }; // class DeprecatedFlyString

    template<>
    struct Traits<DeprecatedFlyString> : public DefaultTraits<DeprecatedFlyString> {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(DeprecatedFlyString const& s) 
        { 
            return s.hash(); 
        }
    }; // struct Traits<DeprecatedFlyString> : public DefaultTraits<DeprecatedFlyString>

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::DeprecatedFlyString;
#endif
