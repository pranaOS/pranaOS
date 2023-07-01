/**
 * @file stringview.h
 * @author Krisna Pranav
 * @brief stringview
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "check.h"
#include "forward.h"
#include "span.h"
#include "stdlibextra.h"
#include "stringutils.h"

namespace Mods {
    class StringView {
    public:
        ALWAYS_INLINE constexpr StringView() {}
        ALWAYS_INLINE constexpr StringView(const char* characters, size_t length)
            : m_characters(characters)
            , m_length(length)
        {
            ASSERT(!Checked<uintptr_t>::addition_would_overflow((uintptr_t)characters, length));
        }

        ALWAYS_INLINE StringView(const unsigned char* characters, size_t length)
            : m_characters((const char*)characters)
            , m_length(length)
        {
            ASSERT(!Checked<uintptr_t>::addition_would_overflow((uintptr_t)characters, length));
        }

        ALWAYS_INLINE constexpr StringView(const char* cstring)
            : m_characters(cstring)
            , m_length(cstring ? __builtin_strlen(cstring) : 0)
        {
        }

        ALWAYS_INLINE StringView(ReadonlyBytes bytes)
            : m_characters(reinterpret_cast<const char*>(bytes.data()))
            , m_length(bytes.size())
        {
        }

        StringView(const ByteBuffer&);
        StringView(const String&);
        StringView(const FlyString&);

        bool is_null() const {
            return !m_characters;
        }

        bool is_empty() const {
            return m_length == 0;
        }
    };
}