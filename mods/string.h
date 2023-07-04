/**
 * @file string.h
 * @author Krisna Pranav
 * @brief string 
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "format.h"
#include "forward.h"
#include "refptr.h"
#include "stream.h"
#include "string_impl.h"
#include "string_utils.h"
#include "traits.h"

namespace Mods {
    class String 
    {
    public:
        ~String() {}

        String(const StringView&);

        /**
         * @param other 
         */
        String(const String& other) : m_impl(const_cast<String&>(other).m_impl)
        {}

        /**
         * @param other 
         */
        String(String&& other) : m_impl(move(other.m_impl)) {}

        /**
         * @brief Construct a new String object
         * 
         * @param cstring 
         * @param shouldChomp 
         */
        String(const char* cstring, ShouldChomp shouldChomp = NoChomp) : m_impl(StringImpl::create(cstring, shouldChomp)) {}

    private:
        bool is_one_of() const {
            return false;
        }

        RefPtr<StringImpl> m_impl;
    };
}