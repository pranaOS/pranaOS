/**
 * @file string.h
 * @author Krisna Pranav
 * @brief string 
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/forward.h>
#include <mods/refptr.h>
#include <mods/stream.h>
#include <mods/string_builder.h>
#include <mods/string_impl.h>
#include <mods/string_utils.h>
#include <mods/traits.h>

namespace Mods
{
    class String 
    {
    public:
        /**
         * @brief Destroy the String object
         * 
         */
        ~String() = default;

        /**
         * @brief Construct a new String object
         * 
         */
        String() = default;

        /**
         * @brief Construct a new String object
         * 
         * @param view 
         */
        String(StringView view)
            : m_impl(StringImpl::create(view.characters_without_null_termination(), view.length()))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param other 
         */
        String(const String& other)
            : m_impl(const_cast<String&>(other).m_impl)
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param other 
         */
        String(String&& other)
            : m_impl(move(other.m_impl))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param cstring 
         * @param shouldChomp 
         */
        String(const char* cstring, ShouldChomp shouldChomp = NoChomp)
            : m_impl(StringImpl::create(cstring, shouldChomp))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param cstring 
         * @param length 
         * @param shouldChomp 
         */
        String(const char* cstring, size_t length, ShouldChomp shouldChomp = NoChomp)
            : m_impl(StringImpl::create(cstring, length, shouldChomp))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param bytes 
         * @param shouldChomp 
         */
        explicit String(ReadonlyBytes bytes, ShouldChomp shouldChomp = NoChomp)
            : m_impl(StringImpl::create(bytes, shouldChomp))
        {
        }   

        /**
         * @brief Construct a new String object
         * 
         * @param impl 
         */
        String(const StringImpl& impl)
            : m_impl(const_cast<StringImpl&>(impl))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param impl 
         */
        String(const StringImpl* impl)
            : m_impl(const_cast<StringImpl*>(impl))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param impl 
         */
        String(RefPtr<StringImpl>&& impl)
            : m_impl(move(impl))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param impl 
         */
        String(NonnullRefPtr<StringImpl>&& impl)
            : m_impl(move(impl))
        {
        }

        String(const FlyString&);

        /**
         * @param count 
         * @return String 
         */
        [[nodiscard]] static String repeated(char, size_t count);

        /**
         * @param count 
         * @return String 
         */
        [[nodiscard]] static String repeated(StringView, size_t count);

        /**
         * @param value 
         * @param base 
         * @param map 
         * @return String 
         */
        [[nodiscard]] static String bijective_base_from(size_t value, unsigned base = 26, StringView map = {});

        /**
         * @param value 
         * @return String 
         */
        [[nodiscard]] static String roman_number_from(size_t value);

        /**
         * @tparam SeparatorType 
         * @tparam CollectionType 
         * @param separator 
         * @param collection 
         * @return String 
         */
        template<class SeparatorType, class CollectionType>
        [[nodiscard]] static String join(const SeparatorType& separator, const CollectionType& collection)
        {
            StringBuilder builder;
            builder.join(separator, collection);
            return builder.build();
        }

        /**
         * @param mask 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool matches(StringView mask, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;

        /**
         * @param mask 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool matches(StringView mask, Vector<MaskSpan>&, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;

        template<typename T = int>
        [[nodiscard]] Optional<T> to_int(TrimWhitespace = TrimWhitespace::Yes) const;

        template<typename T = unsigned>
        [[nodiscard]] Optional<T> to_uint(TrimWhitespace = TrimWhitespace::Yes) const;

        [[nodiscard]] String to_lowercase() const;
        [[nodiscard]] String to_uppercase() const;
        [[nodiscard]] String to_snakecase() const;
        [[nodiscard]] String to_titlecase() const;

        [[nodiscard]] bool is_whitespace() const 
        { 
            return StringUtils::is_whitespace(*this); 
        }

    #ifndef KERNEL
        /**         
         * @param characters 
         * @param mode 
         * @return String 
         */
        [[nodiscard]] String trim(StringView characters, TrimMode mode = TrimMode::Both) const
        {
            auto trimmed_view = StringUtils::trim(view(), characters, mode);
            if (view() == trimmed_view)
                return *this;
            return trimmed_view;
        }
        
        /**
         * @param mode 
         * @return String 
         */
        [[nodiscard]] String trim_whitespace(TrimMode mode = TrimMode::Both) const
        {
            auto trimmed_view = StringUtils::trim_whitespace(view(), mode);
            if (view() == trimmed_view)
                return *this;
            return trimmed_view;
        }
    #endif

        [[nodiscard]] bool equals_ignoring_case(StringView) const;

        [[nodiscard]] bool contains(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
        [[nodiscard]] bool contains(char, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @param separator 
         * @param limit 
         * @param keep_empty 
         * @return Vector<String> 
         */
        [[nodiscard]] Vector<String> split_limit(char separator, size_t limit, bool keep_empty = false) const;

        /**
         * @param separator 
         * @param keep_empty 
         * @return Vector<String> 
         */
        [[nodiscard]] Vector<String> split(char separator, bool keep_empty = false) const;

        /**
         * @param separator 
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(char separator, bool keep_empty = false) const;

        /**
         * @param needle 
         * @param start 
         * @return Optional<size_t> 
         */
        [[nodiscard]] Optional<size_t> find(char needle, size_t start = 0) const 
        { 
            return StringUtils::find(*this, needle, start); 
        }

        /**
         * @param needle 
         * @param start 
         * @return Optional<size_t> 
         */
        [[nodiscard]] Optional<size_t> find(StringView needle, size_t start = 0) const 
        { 
            return StringUtils::find(*this, needle, start); 
        }

        /**
         * @param needle 
         * @return Optional<size_t> 
         */
        [[nodiscard]] Optional<size_t> find_last(char needle) const 
        { 
            return StringUtils::find_last(*this, needle); 
        }

        /**
         * @param needle 
         * @return Vector<size_t> 
         */
        Vector<size_t> find_all(StringView needle) const;

        using SearchDirection = StringUtils::SearchDirection;

        /**
         * @param needles 
         * @param direction 
         * @return Optional<size_t> 
         */
        [[nodiscard]] Optional<size_t> find_any_of(StringView needles, SearchDirection direction) const { return StringUtils::find_any_of(*this, needles, direction); }

        /**
         * @param start 
         * @param length 
         * @return String 
         */
        [[nodiscard]] String substring(size_t start, size_t length) const;

        /**
         * @param start 
         * @return String 
         */
        [[nodiscard]] String substring(size_t start) const;

        /**
         * @param start 
         * @param length 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view(size_t start, size_t length) const;

        /**
         * @param start 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view(size_t start) const;

        [[nodiscard]] bool is_null() const 
        { 
            return !m_impl; 
        }

        [[nodiscard]] ALWAYS_INLINE bool is_empty() const 
        { 
            return length() == 0; 
        }

        [[nodiscard]] ALWAYS_INLINE size_t length() const 
        { 
            return m_impl ? m_impl->length() : 0; 
        }
        
        [[nodiscard]] ALWAYS_INLINE const char* characters() const 
        { 
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
        [[nodiscard]] ALWAYS_INLINE ReadonlyBytes bytes() const
        {
            if (m_impl) {
                return m_impl->bytes();
            }
            return {};
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        [[nodiscard]] ALWAYS_INLINE const char& operator[](size_t i) const
        {
            VERIFY(!is_null());
            return (*m_impl)[i];
        }

        using ConstIterator = SimpleIterator<const String, const char>;

        [[nodiscard]] constexpr ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }

        [[nodiscard]] constexpr ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        [[nodiscard]] bool starts_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        [[nodiscard]] bool ends_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        [[nodiscard]] bool starts_with(char) const;

        [[nodiscard]] bool ends_with(char) const;

        bool operator==(const String&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const String& other) const 
        { 
            return !(*this == other); 
        }

        bool operator==(StringView) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(StringView other) const 
        { 
            return !(*this == other); 
        }

        bool operator==(const FlyString&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const FlyString& other) const 
        { 
            return !(*this == other); 
        }

        bool operator<(const String&) const;

        bool operator<(const char*) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const String& other) const 
        { 
            return !(*this < other); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const char* other) const 
        { 
            return !(*this < other); 
        }

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
        bool operator<=(const String& other) const 
        { 
            return !(*this > other); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(const char* other) const 
        { 
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
        bool operator!=(const char* cstring) const 
        { 
            return !(*this == cstring); 
        }

        [[nodiscard]] String isolated_copy() const;

        [[nodiscard]] static String empty()
        {
            return StringImpl::the_empty_stringimpl();
        }

        [[nodiscard]] StringImpl* impl() 
        { 
            return m_impl.ptr(); 
        }

        [[nodiscard]] const StringImpl* impl() const 
        { 
            return m_impl.ptr(); 
        }

        /**
         * @param other 
         * @return String& 
         */
        String& operator=(String&& other)
        {
            if (this != &other)
                m_impl = move(other.m_impl);
            return *this;
        }       

        /**
         * @param other 
         * @return String& 
         */
        String& operator=(const String& other)
        {
            if (this != &other)
                m_impl = const_cast<String&>(other).m_impl;
            return *this;
        }

        /**
         * @return String& 
         */
        String& operator=(std::nullptr_t)
        {
            m_impl = nullptr;
            return *this;
        }

        /**
         * @param bytes 
         * @return String& 
         */
        String& operator=(ReadonlyBytes bytes)
        {
            m_impl = StringImpl::create(bytes);
            return *this;
        }

        [[nodiscard]] u32 hash() const
        {
            if (!m_impl)
                return 0;
            return m_impl->hash();
        }

        [[nodiscard]] ByteBuffer to_byte_buffer() const;

        /**
         * @tparam BufferType 
         * @param buffer 
         * @param should_chomp 
         * @return String 
         */
        template<typename BufferType>
        [[nodiscard]] static String copy(const BufferType& buffer, ShouldChomp should_chomp = NoChomp)
        {
            if (buffer.is_empty())
                return empty();
            return String((const char*)buffer.data(), buffer.size(), should_chomp);
        }

        /**
         * @param fmtstr 
         * @return String 
         */
        [[nodiscard]] static String vformatted(StringView fmtstr, TypeErasedFormatParams&);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         * @return String 
         */
        template<typename... Parameters>
        [[nodiscard]] static String formatted(CheckedFormatString<Parameters...>&& fmtstr, const Parameters&... parameters)
        {
            VariadicFormatParams variadic_format_parameters { parameters... };
            return vformatted(fmtstr.view(), variadic_format_parameters);
        }

        /**
         * @tparam T 
         * @param value 
         * @return String 
         */
        template<typename T>
        [[nodiscard]] static String number(T value) requires IsArithmetic<T>
        {
            return formatted("{}", value);
        }

        [[nodiscard]] StringView view() const
        {
            return { characters(), length() };
        }

        /**
         * @param needle 
         * @param replacement 
         * @param all_occurrences 
         * @return String 
         */
        [[nodiscard]] String replace(StringView needle, StringView replacement, bool all_occurrences = false) const 
        { 
            return StringUtils::replace(*this, needle, replacement, all_occurrences); 
        }

        /**
         * @param needle 
         * @return size_t 
         */
        [[nodiscard]] size_t count(StringView needle) const 
        { 
            return StringUtils::count(*this, needle); 
        }

        [[nodiscard]] String reverse() const;

        /**
         * @tparam Ts 
         * @param strings 
         * @return ALWAYS_INLINE constexpr 
         */
        template<typename... Ts>
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_one_of(Ts&&... strings) const
        {
            return (... || this->operator==(forward<Ts>(strings)));
        }

    private:
        RefPtr<StringImpl> m_impl;
    }; // class String

    template<>
    struct Traits<String> : public GenericTraits<String> 
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(const String& s) 
        { 
            return s.impl() ? s.impl()->hash() : 0; 
        }
    };

    struct CaseInsensitiveStringTraits : public Traits<String> 
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(String const& s) 
        { 
            return s.impl() ? s.impl()->case_insensitive_hash() : 0; 
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(String const& a, String const& b) 
        { 
            return a.equals_ignoring_case(b); 
        }
    };

    bool operator<(const char*, const String&);
    bool operator>=(const char*, const String&);
    bool operator>(const char*, const String&);
    bool operator<=(const char*, const String&);

    /**
     * @param html 
     * @return String 
     */
    String escape_html_entities(StringView html);

    /**
     * @param stream 
     * @param string 
     * @return InputStream& 
     */
    InputStream& operator>>(InputStream& stream, String& string);
} // namespace Mods

using Mods::CaseInsensitiveStringTraits;
using Mods::escape_html_entities;
using Mods::String;
