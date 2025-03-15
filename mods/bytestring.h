/**
 * @file bytestring.h
 * @author Krisna Pranav
 * @brief byte string
 * @version 6.0
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/forward.h>
#include <mods/refptr.h>
#include <mods/stringbuilder.h>
#include <mods/stringimpl.h>
#include <mods/stringutils.h>
#include <mods/traits.h>

namespace Mods
{

    class ByteString 
    {
    public:
        /**
         * @brief Destroy the Byte String object
         * 
         */
        ~ByteString() = default;

        /**
         * @brief Construct a new Byte String object
         * 
         */
        ByteString()
            : m_impl(StringImpl::the_empty_stringimpl())
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param view 
         */
        ByteString(StringView view)
            : m_impl(*StringImpl::create(view.characters_without_null_termination(), view.length()))
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param other 
         */
        ByteString(ByteString const& other)
            : m_impl(other.m_impl)
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param other 
         */
        ByteString(ByteString&& other)
            : m_impl(move(other.m_impl))
        {
            other.m_impl = StringImpl::the_empty_stringimpl();
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param cstring 
         * @param shouldChomp 
         */
        ByteString(char const* cstring, ShouldChomp shouldChomp = NoChomp)
            : m_impl(*StringImpl::create(cstring, shouldChomp))
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param cstring 
         * @param length 
         * @param shouldChomp 
         */
        ByteString(char const* cstring, size_t length, ShouldChomp shouldChomp = NoChomp)
            : m_impl(*StringImpl::create(cstring, length, shouldChomp))
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param bytes 
         * @param shouldChomp 
         */
        explicit ByteString(ReadonlyBytes bytes, ShouldChomp shouldChomp = NoChomp)
            : m_impl(*StringImpl::create(bytes, shouldChomp))
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param impl 
         */
        ByteString(StringImpl const& impl)
            : m_impl(impl)
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         * @param impl 
         */
        ByteString(NonnullRefPtr<StringImpl const>&& impl)
            : m_impl(*move(impl))
        {
        }

        /**
         * @brief Construct a new Byte String object
         * 
         */
        ByteString(DeprecatedFlyString const&);

        static ErrorOr<ByteString> from_utf8(ReadonlyBytes);

        /**
         * @param string 
         * @return ErrorOr<ByteString> 
         */
        static ErrorOr<ByteString> from_utf8(StringView string) 
        { 
            return from_utf8(string.bytes()); 
        }

        /**
         * @param string 
         * @return ByteString 
         */
        static ByteString must_from_utf8(StringView string) 
        { 
            return MUST(from_utf8(string)); 
        }

        /**
         * @param string 
         * @return ByteString 
         */
        static ByteString from_utf8_without_validation(StringView string) 
        { 
            return ByteString { string }; 
        }

        /**
         * @brief Create a and overwrite object
         * 
         * @tparam F 
         * @tparam PossiblyErrorOr 
         * @tparam is_error_or 
         * @tparam ErrorOr> 
         * @tparam ReturnType 
         * @tparam ErrorOr<ByteString> 
         * @tparam ByteString> 
         * @param length 
         * @param fill_function 
         * @return ReturnType 
         */
        template<
            typename F,
            typename PossiblyErrorOr = decltype(declval<F>()(declval<Bytes>())),
            bool is_error_or = IsSpecializationOf<PossiblyErrorOr, ErrorOr>,
            typename ReturnType = Conditional<is_error_or, ErrorOr<ByteString>, ByteString>>
        static ReturnType create_and_overwrite(size_t length, F&& fill_function)
        {
            char* buffer;
            auto impl = StringImpl::create_uninitialized(length, buffer);

            if constexpr (is_error_or)
                TRY(fill_function(Bytes { buffer, length }));
            else
                fill_function(Bytes { buffer, length });
            return impl;
        }

        /**
         * @param count 
         * @return ByteString 
         */
        [[nodiscard]] static ByteString repeated(char, size_t count);

        /**
         * @param count 
         * @return ByteString 
         */
        [[nodiscard]] static ByteString repeated(StringView, size_t count);

        /**
         * @param value 
         * @param base 
         * @param map 
         * @return ByteString 
         */
        [[nodiscard]] static ByteString bijective_base_from(size_t value, unsigned base = 26, StringView map = {});

        /**
         * @param value 
         * @return ByteString 
         */
        [[nodiscard]] static ByteString roman_number_from(size_t value);

        /**
         * @tparam SeparatorType 
         * @tparam CollectionType 
         * @param separator 
         * @param collection 
         * @param fmtstr 
         * @return ByteString 
         */
        template<class SeparatorType, class CollectionType>
        [[nodiscard]] static ByteString join(SeparatorType const& separator, CollectionType const& collection, StringView fmtstr = "{}"sv)
        {
            StringBuilder builder;
            builder.join(separator, collection, fmtstr);
            return builder.to_byte_string();
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
         * @param trim_whitespace 
         * @return Optional<T> 
         */
        template<Arithmetic T>
        Optional<T> to_number(TrimWhitespace trim_whitespace = TrimWhitespace::Yes) const
        {
            return view().to_number<T>(trim_whitespace);
        }

        /**
         * @return ByteString 
         */
        [[nodiscard]] ByteString to_lowercase() const;
        [[nodiscard]] ByteString to_uppercase() const;
        [[nodiscard]] ByteString to_snakecase() const;
        [[nodiscard]] ByteString to_titlecase() const;
        [[nodiscard]] ByteString invert_case() const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_whitespace() const 
        { 
            return StringUtils::is_whitespace(*this); 
        }

        /**
         * @return DeprecatedStringCodePointIterator 
         */
        [[nodiscard]] DeprecatedStringCodePointIterator code_points() const;

        /**
         * @param characters 
         * @param mode 
         * @return ByteString 
         */
        [[nodiscard]] ByteString trim(StringView characters, TrimMode mode = TrimMode::Both) const
        {
            auto trimmed_view = StringUtils::trim(view(), characters, mode);
            if (view() == trimmed_view)
                return *this;
            return trimmed_view;
        }

        /**
         * @param mode 
         * @return ByteString 
         */
        [[nodiscard]] ByteString trim_whitespace(TrimMode mode = TrimMode::Both) const
        {
            auto trimmed_view = StringUtils::trim_whitespace(view(), mode);
            if (view() == trimmed_view)
                return *this;
            return trimmed_view;
        }

        [[nodiscard]] bool equals_ignoring_ascii_case(StringView) const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool contains(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
        [[nodiscard]] bool contains(char, CaseSensitivity = CaseSensitivity::CaseSensitive) const;

        /**
         * @param separator 
         * @param limit 
         * @return Vector<ByteString> 
         */
        [[nodiscard]] Vector<ByteString> split_limit(char separator, size_t limit, SplitBehavior = SplitBehavior::Nothing) const;

        /**
         * @param separator 
         * @return Vector<ByteString> 
         */
        [[nodiscard]] Vector<ByteString> split(char separator, SplitBehavior = SplitBehavior::Nothing) const;

        /**
         * @param separator 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(char separator, SplitBehavior = SplitBehavior::Nothing) const&;

        /**
         * @param separator 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(char separator, SplitBehavior = SplitBehavior::Nothing) const&& = delete;

        /**
         * @param separator 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(Function<bool(char)> separator, SplitBehavior = SplitBehavior::Nothing) const&;

        /**
         * @param separator 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(Function<bool(char)> separator, SplitBehavior = SplitBehavior::Nothing) const&& = delete;

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
         * @return Optional<size_t> 
         */
        [[nodiscard]] Optional<size_t> find_last(StringView needle) const 
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
         * @param separator 
         * @return StringView 
         */
        [[nodiscard]] StringView find_last_split_view(char separator) const& 
        { 
            return view().find_last_split_view(separator); 
        }

        /**
         * @param separator 
         * @return StringView 
         */
        [[nodiscard]] StringView find_last_split_view(char separator) const&& = delete;

        /**
         * @param start 
         * @param length 
         * @return ByteString 
         */
        [[nodiscard]] ByteString substring(size_t start, size_t length) const;
        
        /**
         * @param start 
         * @return ByteString 
         */
        [[nodiscard]] ByteString substring(size_t start) const;

        /**
         * @param start 
         * @param length 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view(size_t start, size_t length) const&;

        /**
         * @param start 
         * @param length 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view(size_t start, size_t length) const&& = delete;

        /**
         * @param start 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view(size_t start) const&;

        /**
         * @param start 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view(size_t start) const&& = delete;

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
            return m_impl->length(); 
        }
        
        
        [[nodiscard]] ALWAYS_INLINE char const* characters() const 
        { 
            return m_impl->characters(); 
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
        [[nodiscard]] ALWAYS_INLINE ReadonlyBytes bytes() const& 
        { 
            return m_impl->bytes(); 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE ReadonlyBytes bytes() const&& = delete;

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        [[nodiscard]] ALWAYS_INLINE char const& operator[](size_t i) const
        {
            return (*m_impl)[i];
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE u8 byte_at(size_t i) const
        {
            return bit_cast<u8>((*m_impl)[i]);
        }

        using ConstIterator = SimpleIterator<ByteString const, char const>;

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
        [[nodiscard]] bool starts_with(char) const;
        [[nodiscard]] bool ends_with(char) const;

        bool operator==(ByteString const&) const;

        bool operator==(StringView) const;

        bool operator==(DeprecatedFlyString const&) const;

        bool operator<(ByteString const&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(ByteString const& other) const 
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

        bool operator>(ByteString const&) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(ByteString const& other) const 
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
         * @return ByteString 
         */
        [[nodiscard]] ByteString isolated_copy() const;

        /**
         * @return ByteString 
         */
        [[nodiscard]] static ByteString empty()
        {
            return StringImpl::the_empty_stringimpl();
        }

        /**
         * @return NonnullRefPtr<StringImpl const> 
         */
        [[nodiscard]] NonnullRefPtr<StringImpl const> impl() const 
        { 
            return m_impl; 
        }

        /**
         * @param other 
         * @return ByteString& 
         */
        ByteString& operator=(ByteString&& other)
        {
            if (this != &other)
                m_impl = move(other.m_impl);
            return *this;
        }

        /**
         * @param other 
         * @return ByteString& 
         */
        ByteString& operator=(ByteString const& other)
        {
            if (this != &other)
                m_impl = const_cast<ByteString&>(other).m_impl;
            return *this;
        }

        /**
         * @tparam T 
         * @param bytes 
         * @return ByteString& 
         */
        template<OneOf<ReadonlyBytes, Bytes> T>
        ByteString& operator=(T bytes)
        {
            m_impl = *StringImpl::create(bytes);
            return *this;
        }

        /**
         * @return u32 
         */
        [[nodiscard]] u32 hash() const
        {
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
         * @return ByteString 
         */
        template<typename BufferType>
        [[nodiscard]] static ByteString copy(BufferType const& buffer, ShouldChomp should_chomp = NoChomp)
        {
            if (buffer.is_empty())
                return empty();
            return ByteString(reinterpret_cast<char const*>(buffer.data()), buffer.size(), should_chomp);
        }

        /**
         * @param fmtstr 
         * @return ByteString 
         */
        [[nodiscard]] static ByteString vformatted(StringView fmtstr, TypeErasedFormatParams&);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         * @return ByteString 
         */
        template<typename... Parameters>
        [[nodiscard]] static ByteString formatted(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
        {
            VariadicFormatParams<AllowDebugOnlyFormatters::No, Parameters...> variadic_format_parameters { parameters... };
            return vformatted(fmtstr.view(), variadic_format_parameters);
        }

        /**
         * @tparam T 
         * @param value 
         * @return ByteString 
         */
        template<Arithmetic T>
        [[nodiscard]] static ByteString number(T value)
        {
            return formatted("{}", value);
        }

        /**
         * @return StringView 
         */
        [[nodiscard]] StringView view() const& 
        { 
            return { characters(), length() }; 
        }

        /**
         * @return StringView 
         */
        [[nodiscard]] StringView view() const&& = delete;

        /**
         * @param needle 
         * @param replacement 
         * @param replace_mode 
         * @return ByteString 
         */
        [[nodiscard]] ByteString replace(StringView needle, StringView replacement, ReplaceMode replace_mode = ReplaceMode::All) const { return StringUtils::replace(*this, needle, replacement, replace_mode); }

        /**
         * @param needle 
         * @return size_t 
         */
        [[nodiscard]] size_t count(StringView needle) const 
        { 
            return StringUtils::count(*this, needle); 
        }

        /**
         * @return ByteString 
         */
        [[nodiscard]] ByteString reverse() const;

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

        /**
         * @tparam Ts 
         * @param strings 
         * @return ALWAYS_INLINE constexpr 
         */
        template<typename... Ts>
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_one_of_ignoring_ascii_case(Ts&&... strings) const
        {
            return (... ||
                    [this, &strings]() -> bool {
                if constexpr (requires(Ts a) { a.view()->StringView; })
                    return this->equals_ignoring_ascii_case(forward<Ts>(strings.view()));
                else
                    return this->equals_ignoring_ascii_case(forward<Ts>(strings));
            }());
        }

    private:
        NonnullRefPtr<StringImpl const> m_impl;
    }; // class ByteString

    /**
     * @tparam  
     */
    template<>
    struct Traits<ByteString> : public DefaultTraits<ByteString> {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(ByteString const& s) 
        { 
            return s.impl()->hash(); 
        }
    };

    struct CaseInsensitiveStringTraits : public Traits<ByteString> {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(ByteString const& s) 
        { 
            return s.impl()->case_insensitive_hash(); 
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(ByteString const& a, ByteString const& b) 
        { 
            return a.equals_ignoring_ascii_case(b); 
        }
    }; // struct CaseInsensitiveStringTraits : public Traits<ByteString>

    ByteString escape_html_entities(StringView html);

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::CaseInsensitiveStringTraits;
using Mods::escape_html_entities;
#endif
