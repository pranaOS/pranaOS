/**
 * @file string.h
 * @author Krisna Pranav
 * @brief string 
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/refptr.h>
#include <mods/stream.h>
#include <mods/traits.h>
#include <mods/string_impl.h>
#include <mods/string_utils.h>
#include <mods/format.h>

namespace Mods {

    class String {
    public: 
        /**
         * @brief Destroy the String object
         * 
         */
        ~String() { }

        String() {}
        String(const StringView&);

        /**
         * @param other 
         */
        String(const String& other) : m_impl(const_cast<String&>(other).m_impl) {}

        /**
         * @param other 
         */
        String(String&& other) : m_impl(move(other.m_impl))
        {}

        /**
         * @param cstring 
         * @param shouldChomp 
         */
        String(const char* cstring, ShouldChomp shouldChomp = NoChomp) : m_impl(StringImpl::create(cstring, shouldChomp))
        { }

        /**
         * @param cstring 
         * @param length 
         * @param shouldChomp 
         */
        String(const char* cstring, size_t length, ShouldChomp shouldChomp = NoChomp) : m_impl(StringImpl::create(cstring, length, shouldChomp))
        {
        }

        /**
         * @param bytes 
         * @param shouldChomp 
         */
        explicit String(ReadonlyBytes bytes, ShouldChomp shouldChomp = NoChomp) : m_impl(StringImpl::create(bytes, shouldChomp))
        {
        }   

        /**
         * @param impl 
         */
        String(const StringImpl& impl) : m_impl(const_cast<StringImpl&>(impl))
        {
        }

        /**
         * @param impl 
         */
        String(const StringImpl* impl) : m_impl(const_cast<StringImpl*>(impl))
        {
        }

        /**
         * @param impl 
         */
        String(RefPtr<StringImpl>&& impl)
            : m_impl(move(impl))
        {
        }

        /**
         * @param impl 
         */
        String(NonnullRefPtr<StringImpl>&& impl) : m_impl(move(impl))
        {
        }

        String(const FlyString&);

        /**
         * @param count 
         * @return String 
         */
        static String repeated(char, size_t count);

        /**
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
         * @return Optional<int> 
         */
        Optional<int> to_int() const;

        /**
         * @return Optional<unsigned> 
         */
        Optional<unsigned> to_uint() const;

        /**
         * @return String 
         */
        String to_lowercase() const;
        String to_uppercase() const;

    #ifndef KERNEL
        /**
         * @param mode 
         * @return String 
         */
        String trim_whitespace(TrimMode mode = TrimMode::Both) const {
            return StringUtils::trim_whitespace(StringView { characters(), length() }, mode);
        }
    #endif

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(const StringView&) const;

        /** 
         * @return true 
         * @return false 
         */
        bool contains(const StringView&, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @param start 
         * @return Optional<size_t> 
         */
        Optional<size_t> index_of(const String&, size_t start = 0) const;

        /**
         * @param separator 
         * @param limit 
         * @param keep_empty 
         * @return Vector<String> 
         */
        Vector<String> split_limit(char separator, size_t limit, bool keep_empty = false) const;

        /**
         * @param separator 
         * @param keep_empty 
         * @return Vector<String> 
         */
        Vector<String> split(char separator, bool keep_empty = false) const;

        /**
         * @param start 
         * @return String 
         */
        String substring(size_t start) const;

        /**
         * @param start 
         * @param length 
         * @return String 
         */
        String substring(size_t start, size_t length) const;

        /** 
         * @param separator 
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        Vector<StringView> split_view(char separator, bool keep_empty = false) const;

        /**
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
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return !m_impl; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_empty() const { 
            return length() == 0; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE size_t length() const { 
            return m_impl ? m_impl->length() : 0; 
        }
        
        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE const char* characters() const { 
            return m_impl ? m_impl->characters() : nullptr; 
        }
        
        /**
         * @param buffer 
         * @param buffer_size 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool copy_characters_to_buffer(char* buffer, size_t buffer_size) const;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ReadonlyBytes bytes() const { 
            return m_impl ? m_impl->bytes() : nullptr; 
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const char& operator[](size_t i) const {
            return (*m_impl)[i];
        }

        using ConstIterator = SimpleIterator<const String, const char>;

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
         * @return true 
         * @return false 
         */
        bool starts_with(char) const;

        /**
         * @return true 
         * @return false 
         */
        bool ends_with(char) const;

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
        bool operator!=(const String& other) const { 
            return !(*this == other); 
        }

        /** 
         * @return true 
         * @return false 
         */
        bool operator==(const StringView&) const;

        /** 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const StringView& other) const { 
            return !(*this == other); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const FlyString&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const FlyString& other) const { 
            return !(*this == other); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator<(const String&) const;

        /**
         * @return true 
         * @return false 
         */
        bool operator<(const char*) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const String& other) const { 
            return !(*this < other); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const char* other) const { 
            return !(*this < other); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator>(const String&) const;

        /** 
         * @return true 
         * @return false 
         */
        bool operator>(const char*) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(const String& other) const { 
            return !(*this > other); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(const char* other) const { 
            return !(*this > other); 
        }

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator==(const char* cstring) const;

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator!=(const char* cstring) const { 
            return !(*this == cstring); 
        }

        String isolated_copy() const;

        static String empty();

        /**
         * @return StringImpl* 
         */
        StringImpl* impl() { 
            return m_impl.ptr(); 
        }

        /**
         * @return const StringImpl* 
         */
        const StringImpl* impl() const { 
            return m_impl.ptr(); 
        }

        /**
         * @param other 
         * @return String& 
         */
        String& operator=(String&& other) {
            if (this != &other)
                m_impl = move(other.m_impl);
            return *this;
        }

        /**
         * @param other 
         * @return String& 
         */
        String& operator=(const String& other) {
            if (this != &other)
                m_impl = const_cast<String&>(other).m_impl;
            return *this;
        }

        /**
         * @return String& 
         */
        String& operator=(std::nullptr_t) {
            m_impl = nullptr;
            return *this;
        }

        /**
         * @param bytes 
         * @return String& 
         */
        String& operator=(ReadonlyBytes bytes) {
            m_impl = StringImpl::create(bytes);
            return *this;
        }

        /**
         * @return u32 
         */
        u32 hash() const {
            if (!m_impl)
                return 0;
            return m_impl->hash();
        }

        /**
         * @return ByteBuffer 
         */
        ByteBuffer to_byte_buffer() const;

        /**
         * @tparam BufferType 
         * @param buffer 
         * @param should_chomp 
         * @return String 
         */
        template<typename BufferType>
        static String copy(const BufferType& buffer, ShouldChomp should_chomp = NoChomp) {
            if (buffer.is_null())
                return {};
            if (buffer.is_empty())
                return empty();
            return String((const char*)buffer.data(), buffer.size(), should_chomp);
        }

        /** 
         * @param ... 
         * @return String 
         */
        static String format(const char*, ...);

        /**
         * @param fmtstr 
         * @return String 
         */
        static String vformatted(StringView fmtstr, TypeErasedFormatParams);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         * @return String 
         */
        template<typename... Parameters>
        static String formatted(StringView fmtstr, const Parameters&... parameters) {
            return vformatted(fmtstr, VariadicFormatParams { parameters... });
        }
        
        /**
         * @tparam T 
         * @return String 
         */
        template<typename T>
        static String number(T);

        /**
         * @return StringView 
         */
        StringView view() const;

        /**
         * @param needle 
         * @param replacement 
         * @param all_occurrences 
         * @return int 
         */
        int replace(const String& needle, const String& replacement, bool all_occurrences = false);

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
    struct Traits<String> : public GenericTraits<String> {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(const String& s) { 
            return s.impl() ? s.impl()->hash() : 0; 
        }
    };

    struct CaseInsensitiveStringTraits : public Mods::Traits<String> {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(const String& s) { 
            return s.impl() ? s.to_lowercase().impl()->hash() : 0; 
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(const String& a, const String& b) { 
            return a.to_lowercase() == b.to_lowercase(); 
        }
    };

    /**
     * @return true 
     * @return false 
     */
    bool operator<(const char*, const String&);

    /**
     * @return true 
     * @return false 
     */
    bool operator>=(const char*, const String&);

    /**
     * @return true 
     * @return false 
     */
    bool operator>(const char*, const String&);

    /**
     * @return true 
     * @return false 
     */
    bool operator<=(const char*, const String&);

    /**
     * @param html 
     * @return String 
     */
    String escape_html_entities(const StringView& html);

    /**
     * @param stream 
     * @param string 
     * @return InputStream& 
     */
    InputStream& operator>>(InputStream& stream, String& string);

}

using Mods::CaseInsensitiveStringTraits;
using Mods::escape_html_entities;
using Mods::String;
