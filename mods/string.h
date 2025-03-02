/**
 * @file string.h
 * @author Krisna Pranav
 * @brief string
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/format.h>
#include <mods/forward.h>
#include <mods/refptr.h>
#include <mods/stream.h>
#include <mods/stringbuilder.h>
#include <mods/stringimpl.h>
#include <mods/stringutils.h>
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
        String(String const& other)
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
        String(char const* cstring, ShouldChomp shouldChomp = NoChomp)
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
        String(char const* cstring, size_t length, ShouldChomp shouldChomp = NoChomp)
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
        String(StringImpl const& impl)
            : m_impl(const_cast<StringImpl&>(impl))
        {
        }

        /**
         * @brief Construct a new String object
         * 
         * @param impl 
         */
        String(StringImpl const* impl)
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

        /**
         * @brief Construct a new String object
         * 
         */
        String(FlyString const&);

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
         * @param fmtstr 
         * @return String 
         */
        template <class SeparatorType, class CollectionType>
        [[nodiscard]] static String join(SeparatorType const& separator, CollectionType const& collection, StringView fmtstr = "{}"sv)
        {
            StringBuilder builder;
            builder.join(separator, collection, fmtstr);
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

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template <typename T = int>
        [[nodiscard]] Optional<T> to_int(TrimWhitespace = TrimWhitespace::Yes) const;

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template <typename T = unsigned>
        [[nodiscard]] Optional<T> to_uint(TrimWhitespace = TrimWhitespace::Yes) const;

        /**
         * @return String 
         */
        [[nodiscard]] String to_lowercase() const;
        [[nodiscard]] String to_uppercase() const;
        [[nodiscard]] String to_snakecase() const;
        [[nodiscard]] String to_titlecase() const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_whitespace() const
        {
            return StringUtils::is_whitespace(*this);
        }

        /**
         * @param characters 
         * @param mode 
         * @return String 
         */
        [[nodiscard]] String trim(StringView characters, TrimMode mode = TrimMode::Both) const
        {
            auto trimmed_view = StringUtils::trim(view(), characters, mode);
            if(view() == trimmed_view)
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
            if(view() == trimmed_view)
                return *this;
            return trimmed_view;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool equals_ignoring_case(StringView) const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool contains(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @return true 
         * @return false 
         */
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
         * @param separator 
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(Function<bool(char)> separator, bool keep_empty = false) const;

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
        [[nodiscard]] Optional<size_t> find_any_of(StringView needles, SearchDirection direction) const
        {
            return StringUtils::find_any_of(*this, needles, direction);
        }

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

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_null() const
        {
            return !m_impl;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_empty() const
        {
            return length() == 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE size_t length() const
        {
            return m_impl ? m_impl->length() : 0;
        }
        
        /**
         * @return ALWAYS_INLINE const* 
         */
        [[nodiscard]] ALWAYS_INLINE char const* characters() const
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
            if(m_impl)
            {
                return m_impl->bytes();
            }
            return {};
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        [[nodiscard]] ALWAYS_INLINE char const& operator[](size_t i) const
        {
            VERIFY(!is_null());
            return (*m_impl)[i];
        }

        using ConstIterator = SimpleIterator<const String, char const>;

        /**
         * @return constexpr ConstIterator 
         */
        [[nodiscard]] constexpr ConstIterator begin() const
        {
            return ConstIterator::begin(*this);
        }
        [[nodiscard]] constexpr ConstIterator end() const
        {
            return ConstIterator::end(*this);
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool starts_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
        [[nodiscard]] bool ends_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool starts_with(char) const;
        [[nodiscard]] bool ends_with(char) const;

        /**
         * @return true 
         * @return false 
         */
        bool operator==(String const&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(String const& other) const
        {
            return !(*this == other);
        }

        /**
         * @return true 
         * @return false 
         */
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

        /**
         * @return true 
         * @return false 
         */
        bool operator==(FlyString const&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(FlyString const& other) const
        {
            return !(*this == other);
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator<(String const&) const;

        /**
         * @return true 
         * @return false 
         */
        bool operator<(char const*) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(String const& other) const
        {
            return !(*this < other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(char const* other) const
        {
            return !(*this < other);
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator>(String const&) const;

        /**
         * @return true 
         * @return false 
         */
        bool operator>(char const*) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(String const& other) const
        {
            return !(*this > other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(char const* other) const
        {
            return !(*this > other);
        }

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator==(char const* cstring) const;

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator!=(char const* cstring) const
        {
            return !(*this == cstring);
        }

        /**
         * @return String 
         */
        [[nodiscard]] String isolated_copy() const;

        /**
         * @return String 
         */
        [[nodiscard]] static String empty()
        {
            return StringImpl::the_empty_stringimpl();
        }

        /**
         * @return StringImpl* 
         */
        [[nodiscard]] StringImpl* impl()
        {
            return m_impl.ptr();
        }

        /**
         * @return StringImpl const* 
         */
        [[nodiscard]] StringImpl const* impl() const
        {
            return m_impl.ptr();
        }   

        /**
         * @param other 
         * @return String& 
         */
        String& operator=(String&& other)
        {
            if(this != &other)
                m_impl = move(other.m_impl);
            return *this;
        }   

        /**
         * @param other 
         * @return String& 
         */
        String& operator=(String const& other)
        {
            if(this != &other)
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

        /**
         * @return u32 
         */
        [[nodiscard]] u32 hash() const
        {
            if(!m_impl)
                return 0;
            return m_impl->hash();
        }

        /**
         * @return ByteBuffer 
         */
        [[nodiscard]] ByteBuffer to_byte_buffer() const;

        /**
         * @tparam BufferType 
         * @param buffer 
         * @param should_chomp 
         * @return String 
         */
        template <typename BufferType>
        [[nodiscard]] static String copy(BufferType const& buffer, ShouldChomp should_chomp = NoChomp)
        {
            if(buffer.is_empty())
                return empty();
            return String((char const*)buffer.data(), buffer.size(), should_chomp);
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
        template <typename... Parameters>
        [[nodiscard]] static String formatted(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
        {
            VariadicFormatParams variadic_format_parameters{parameters...};
            return vformatted(fmtstr.view(), variadic_format_parameters);
        }

        /**
         * @tparam T 
         * @param value 
         * @return String 
         */
        template <typename T>
        [[nodiscard]] static String number(T value)
            requires IsArithmetic<T>
        {
            return formatted("{}", value);
        }

        /**
         * @return StringView 
         */
        [[nodiscard]] StringView view() const
        {
            return {characters(), length()};
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
        template <typename... Ts>
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_one_of(Ts&&... strings) const
        {
            return (... || this->operator==(forward<Ts>(strings)));
        }

        /**
         * @tparam Ts 
         * @param strings 
         * @return ALWAYS_INLINE constexpr 
         */
        template <typename... Ts>
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_one_of_ignoring_case(Ts&&... strings) const
        {
            return (... ||
                        [this, &strings]() -> bool
                    {
                        if constexpr(requires(Ts a) { a.view()->StringView; })
                            return this->equals_ignoring_case(forward<Ts>(strings.view()));
                        else
                            return this->equals_ignoring_case(forward<Ts>(strings));
                    }());
        }

    private:
        RefPtr<StringImpl> m_impl;
    }; // class String

    template <>
    struct Traits<String> : public GenericTraits<String>
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(String const& s)
        {
            return s.impl() ? s.impl()->hash() : 0;
        }
    }; // struct Traits<String> : public GenericTraits<String>

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
    }; // struct CaseInsensitiveStringTraits : public Traits<String>

    /**
     * @return true 
     * @return false 
     */
    bool operator<(char const*, String const&);

    /**
     * @return true 
     * @return false 
     */
    bool operator>=(char const*, String const&);
    bool operator>(char const*, String const&);
    bool operator<=(char const*, String const&);

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