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

#include "mods/stringutils.h"
#include <mods/string.h>

namespace Mods 
{
    class FlyString 
    {
    public:
        /**
         * @brief Construct a new Fly String object
         * 
         */
        FlyString() = default;

        /**
         * @brief Construct a new Fly String object
         * 
         * @param other 
         */
        FlyString(FlyString const& other)
            : m_impl(other.impl())
        {
        }

        /**
         * @brief Construct a new Fly String object
         * 
         * @param other 
         */
        FlyString(FlyString&& other)
            : m_impl(move(other.m_impl))
        {
        }

        /**
         * @brief Construct a new Fly String object
         * 
         */
        FlyString(String const&);

        /**
         * @brief Construct a new Fly String object
         * 
         */
        FlyString(StringView);

        /**
         * @brief Construct a new Fly String object
         * 
         * @param string 
         */
        FlyString(char const* string)
            : FlyString(static_cast<String>(string))
        {
        }

        /**
         * @param impl 
         * @return FlyString 
         */
        static FlyString from_fly_impl(NonnullRefPtr<StringImpl> impl)
        {
            VERIFY(impl->is_fly());
            FlyString string;
            string.m_impl = move(impl);
            return string;
        }

        /**
         * @param other 
         * @return FlyString& 
         */
        FlyString& operator=(FlyString const& other)
        {
            m_impl = other.m_impl;
            return *this;
        }

        /**
         * @param other 
         * @return FlyString& 
         */
        FlyString& operator=(FlyString&& other)
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
            return !m_impl || !m_impl->length(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const 
        { 
            return !m_impl; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(FlyString const& other) const 
        { 
            return m_impl == other.m_impl; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(FlyString const& other) const 
        { 
            return m_impl != other.m_impl; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(String const&) const;

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator!=(String const& string) const 
        { 
            return !(*this == string); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(StringView) const;

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator!=(StringView string) const 
        { 
            return !(*this == string); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(char const*) const;

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator!=(char const* string) const 
        { 
            return !(*this == string); 
        }

        /**
         * @return StringImpl const* 
         */
        StringImpl const* impl() const 
        { 
            return m_impl; 
        }

        /**
         * @return char const* 
         */
        char const* characters() const 
        { 
            return m_impl ? m_impl->characters() : nullptr; 
        }

        /**
         * @return size_t 
         */
        size_t length() const 
        { 
            return m_impl ? m_impl->length() : 0; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 hash() const 
        { 
            return m_impl ? m_impl->existing_hash() : 0; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE StringView view() const 
        { 
            return m_impl ? m_impl->view() : StringView {}; 
        }

        /**
         * @return FlyString 
         */
        FlyString to_lowercase() const;

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template<typename T = int>
        Optional<T> to_int(TrimWhitespace = TrimWhitespace::Yes) const;

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template<typename T = unsigned>
        Optional<T> to_uint(TrimWhitespace = TrimWhitespace::Yes) const;

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(StringView) const;

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
        RefPtr<StringImpl> m_impl;
    }; // class FlyString

    /**
     * @tparam  
     */
    template<>
    struct Traits<FlyString> : public GenericTraits<FlyString> 
    {
        static unsigned hash(FlyString const& s) { return s.hash(); }
    };
} // namespace Mods

using Mods::FlyString;
