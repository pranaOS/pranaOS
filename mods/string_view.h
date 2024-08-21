/**
 * @file stringview.h
 * @author Krisna Pranav
 * @brief stringview
 * @version 6.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        {}

        ALWAYS_INLINE StringView(ReadonlyBytes bytes)
            : m_characters(reinterpret_cast<const char*>(bytes.data()))
            , m_length(bytes.size())
        {}

        /**
         * @brief Construct a new String View object
         * 
         */
        StringView(const ByteBuffer&);
        StringView(const String&);
        StringView(const FlyString&);

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return !m_characters; 
        }
        
        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return m_length == 0; 
        }

        /**
         * @return const char* 
         */
        const char* characters_without_null_termination() const { 
            return m_characters; 
        }

        /**
         * @return size_t 
         */
        size_t length() const { 
            return m_length; 
        }

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes bytes() const { 
            return { m_characters, m_length }; 
        }

        /**
         * @param index 
         * @return const char& 
         */
        const char& operator[](size_t index) const { 
            return m_characters[index]; 
        }

        using ConstIterator = SimpleIterator<const StringView, const char>;

        /**
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator begin() const { 
            return ConstIterator::begin(*this); 
        }

        /**
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator end() const { 
            return ConstIterator::end(*this); 
        }

        unsigned hash() const;

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

        /**
         * @brief starts_with
         * 
         * @return true 
         * @return false 
         */
        bool starts_with(char) const;

        /**
         * @brief ends_with
         * 
         * @return true 
         * @return false 
         */
        bool ends_with(char) const;

        /**
         * @brief matches
         * 
         * @param mask 
         * @return true 
         * @return false 
         */
        bool matches(const StringView& mask, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;

        /**
         * @param mask 
         * @return true 
         * @return false 
         */
        bool matches(const StringView& mask, Vector<MaskSpan>&, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;

        /**
         * @brief contains
         * 
         * @return true 
         * @return false 
         */
        bool contains(char) const;

        /**
         * @return true 
         * @return false 
         */
        bool contains(const StringView&, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(const StringView& other) const;

        /**
         * @param mode 
         * @return StringView 
         */
        StringView trim_whitespace(TrimMode mode = TrimMode::Both) const { 
            return StringUtils::trim_whitespace(*this, mode); 
        }

        /**
         * @return Optional<size_t> 
         */
        Optional<size_t> find_first_of(char) const;
        
        /**
         * @return Optional<size_t> 
         */
        Optional<size_t> find_first_of(const StringView&) const;

        /**
         * @return Optional<size_t> 
         */
        Optional<size_t> find_last_of(char) const;

        /**
         * @return Optional<size_t> 
         */
        Optional<size_t> find_last_of(const StringView&) const;

        /**
         * @brief substring_view
         * 
         * @param start 
         * @param length 
         * @return StringView 
         */
        StringView substring_view(size_t start, size_t length) const;

        /**
         * @param start 
         * @return StringView 
         */
        StringView substring_view(size_t start) const;

        /**
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        Vector<StringView> split_view(char, bool keep_empty = false) const;

        /**
         * @brief split_view
         * 
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        Vector<StringView> split_view(const StringView&, bool keep_empty = false) const;

        /**
         * @param consider_cr 
         * @return Vector<StringView> 
         */
        Vector<StringView> lines(bool consider_cr = true) const;

        /**
         * @brief to_int
         * 
         * @return Optional<int> 
         */
        Optional<int> to_int() const;

        /**
         * @brief to_uint
         * 
         * @return Optional<unsigned> 
         */
        Optional<unsigned> to_uint() const;

        /**
         * @param substring 
         * @return StringView 
         */
        StringView substring_view_starting_from_substring(const StringView& substring) const;
        StringView substring_view_starting_after_substring(const StringView& substring) const;

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator==(const char* cstring) const {
            if (is_null())
                return !cstring;
            if (!cstring)
                return false;
            size_t other_length = __builtin_strlen(cstring);
            if (m_length != other_length)
                return false;
            return !__builtin_memcmp(m_characters, cstring, m_length);
        }

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator!=(const char* cstring) const {
            return !(*this == cstring);
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const String&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const StringView& other) const {
            if (is_null())
                return other.is_null();
            if (other.is_null())
                return false;
            if (length() != other.length())
                return false;
            return !__builtin_memcmp(m_characters, other.m_characters, m_length);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const StringView& other) const {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(const StringView& other) const {
            if (int c = __builtin_memcmp(m_characters, other.m_characters, min(m_length, other.m_length)))
                return c < 0;
            return m_length < other.m_length;
        }

        /**
         * @return const StringImpl* 
         */
        const StringImpl* impl() const { 
            return m_impl; 
        }

        /**
         * @return String 
         */
        String to_string() const;

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

        friend class String;
        const StringImpl* m_impl { nullptr };
        const char* m_characters { nullptr };
        size_t m_length { 0 };
    };
    
    template<>
    struct Traits<StringView> : public GenericTraits<String> {
        static unsigned hash(const StringView& s) { return s.hash(); }
    };

}

// using stringView mods
using Mods::StringView;
