/**
 * @file fly_string.h
 * @author Krisna Pranav
 * @brief fly string
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "string.h"

namespace Mods {

    class FlyString {
    public:

        FlyString() {}

        /**
         * @param other 
         */
        FlyString(const FlyString& other) : m_impl(other.impl())
        {
        }

        /**
         * @param other 
         */
        FlyString(FlyString&& other) : m_impl(move(other.m_impl))
        {
        }

        /**
         * @brief Construct a new Fly String object
         * 
         */
        FlyString(const String&);
        FlyString(const StringView&);
        FlyString(const char*);

        /**
         * @param other 
         * @return FlyString& 
         */
        FlyString& operator=(const FlyString& other) {
            m_impl = other.m_impl;
            return *this;
        }

        /**
         * @param other 
         * @return FlyString& 
         */
        FlyString& operator=(FlyString&& other) {
            m_impl = move(other.m_impl);
            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return !m_impl || !m_impl->length(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return !m_impl; 
        }

        /** 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const FlyString& other) const { 
            return m_impl == other.m_impl; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const FlyString& other) const { 
            return m_impl != other.m_impl; 
        }

        /** 
         * @return true 
         * @return false 
         */
        bool operator==(const String&) const;

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator!=(const String& string) const { 
            return !(*this == string); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const StringView&) const;

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator!=(const StringView& string) const { 
            return !(*this == string); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const char*) const;

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator!=(const char* string) const { 
            return !(*this == string); 
        }

        /**
         * @return const StringImpl* 
         */
        const StringImpl* impl() const { 
            return m_impl; 
        }

        /**
         * @return const char* 
         */
        const char* characters() const { 
            return m_impl ? m_impl->characters() : nullptr; 
        }

        /**
         * @return size_t 
         */
        size_t length() const { 
            return m_impl ? m_impl->length() : 0; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 hash() const { 
            return m_impl ? m_impl->existing_hash() : 0; 
        }

        /**
         * @return StringView 
         */
        StringView view() const;

        /**
         * @return FlyString 
         */
        FlyString to_lowercase() const;

        /**
         * @return Optional<int> 
         */
        Optional<int> to_int() const;

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(const StringView&) const;

        /**
         * @return true 
         * @return false 
         */
        bool starts_with(const StringView&, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @return true 
         * @return false 
         */
        bool ends_with(const StringView&, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        static void did_destroy_impl(Badge<StringImpl>, StringImpl&);

        /**
         * @tparam T 
         * @tparam Rest 
         * @param string 
         * @param rest 
         * @return true 
         * @return false 
         */
        template<typename T, typename... Rest>
        bool is_one_of(const T& string, Rest... rest) const {
            if (*this == string)
                return true;
            return is_one_of(rest...);
        }

    private:   
        /**
         * @return true 
         * @return false 
         */
        bool is_one_of() const { 
            return false; 
        }
        RefPtr<StringImpl> m_impl;
    };
    
    template<>
    struct Traits<FlyString> : public GenericTraits<FlyString> {

        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(const FlyString& s) { 
            return s.hash(); 
        }
    };

}

using Mods::FlyString;
