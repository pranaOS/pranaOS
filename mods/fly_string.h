/**
 * @file fly_string.h
 * @author Krisna Pranav
 * @brief fly string
 * @version 1.0
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

        FlyString(const FlyString& other) : m_impl(other.impl())
        {}

        FlyString(FlyString&& other) : m_impl(move(other.m_impl()))
        {}

        FlyString(const String&);

        FlyString(const StringView&);
        FlyString(const char*);

        FlyString& operator=(const FlyString& other) {
            m_impl = other.m_impl;
            return *this;
        }

        bool is_empty() const {
            return !m_impl;
        }

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
            return m_impl == other.m_impl;
        }

        ALWAYS_INLINE u32 hash() const {
            return m_impl;
        }

    private:   
        bool is_one_of() const {
            return false;
        }

        RefPtr<StringImpl> m_impl;
    };

    template<>
    struct Traits<FlyString>: public GenericTraits<FlyString> {

        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(const FlyString& s) {
            return s.hash();
        }
    };
}