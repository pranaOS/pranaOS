/**
 * @file stringview.h
 * @author Krisna Pranav
 * @brief stringview
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/checked.h>
#include <mods/forward.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <mods/stringhash.h>
#include <mods/stringutils.h>

namespace Mods
{
    class StringView
    {
    public:
        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr StringView() = default;

        /**
         * @param characters 
         * @param length 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr StringView(char const* characters, size_t length)
            : m_characters(characters), m_length(length)
        {
            if(!is_constant_evaluated())
                VERIFY(!Checked<uintptr_t>::addition_would_overflow((uintptr_t)characters, length));
        }

        /**
         * @param characters 
         * @param length 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE StringView(unsigned char const* characters, size_t length)
            : m_characters((char const*)characters), m_length(length)
        {
            VERIFY(!Checked<uintptr_t>::addition_would_overflow((uintptr_t)characters, length));
        }

        /**
         * @param cstring 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr StringView(char const* cstring)
            : m_characters(cstring), m_length(cstring ? __builtin_strlen(cstring) : 0)
        {
        }

        /**
         * @param bytes 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE StringView(ReadonlyBytes bytes)
            : m_characters(reinterpret_cast<char const*>(bytes.data())), m_length(bytes.size())
        {
        }

        /**
         * @brief Construct a new String View object
         * 
         */
        StringView(ByteBuffer const&);
    #ifndef KERNEL
        StringView(String const&);
        StringView(FlyString const&);
    #endif

        explicit StringView(ByteBuffer&&) = delete;
    #ifndef KERNEL
        explicit StringView(String&&) = delete;
        explicit StringView(FlyString&&) = delete;
    #endif

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] constexpr bool is_null() const
        {
            return m_characters == nullptr;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] constexpr bool is_empty() const
        {
            return m_length == 0;
        }

        /**
         * @return constexpr char const* 
         */
        [[nodiscard]] constexpr char const* characters_without_null_termination() const
        {
            return m_characters;
        }

        /**
         * @return constexpr size_t 
         */
        [[nodiscard]] constexpr size_t length() const
        {
            return m_length;
        }

        /**
         * @return ReadonlyBytes 
         */
        [[nodiscard]] ReadonlyBytes bytes() const
        {
            return {m_characters, m_length};
        }

        /**
         * @param index 
         * @return constexpr char const& 
         */
        constexpr char const& operator[](size_t index) const
        {
            return m_characters[index];
        }

        using ConstIterator = SimpleIterator<const StringView, char const>;

        /**
         * @return constexpr ConstIterator 
         */
        [[nodiscard]] constexpr ConstIterator begin() const
        {
            return ConstIterator::begin(*this);
        }

        /**
         * @return constexpr ConstIterator 
         */
        [[nodiscard]] constexpr ConstIterator end() const
        {
            return ConstIterator::end(*this);
        }

        /**
         * @return constexpr unsigned 
         */
        [[nodiscard]] constexpr unsigned hash() const
        {
            if(is_empty())
                return 0;
            return string_hash(characters_without_null_termination(), length());
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool starts_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
        [[nodiscard]] bool ends_with(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
        [[nodiscard]] bool starts_with(char) const;
        [[nodiscard]] bool ends_with(char) const;
        [[nodiscard]] bool matches(StringView mask, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;
        [[nodiscard]] bool matches(StringView mask, Vector<MaskSpan>&, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;
        [[nodiscard]] bool contains(char) const;
        [[nodiscard]] bool contains(StringView, CaseSensitivity = CaseSensitivity::CaseSensitive) const;
        [[nodiscard]] bool equals_ignoring_case(StringView other) const;

        /**
         * @param characters 
         * @param mode 
         * @return StringView 
         */
        [[nodiscard]] StringView trim(StringView characters, TrimMode mode = TrimMode::Both) const
        {
            return StringUtils::trim(*this, characters, mode);
        }

        /**
         * @param mode 
         * @return StringView 
         */
        [[nodiscard]] StringView trim_whitespace(TrimMode mode = TrimMode::Both) const
        {
            return StringUtils::trim_whitespace(*this, mode);
        }

    #ifndef KERNEL
        [[nodiscard]] String to_lowercase_string() const;
        [[nodiscard]] String to_uppercase_string() const;
        [[nodiscard]] String to_titlecase_string() const;
    #endif

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
        [[nodiscard]] Vector<size_t> find_all(StringView needle) const;

        using SearchDirection = StringUtils::SearchDirection;

        /**
         * @param needles 
         * @param direction 
         * @return Optional<size_t> 
         */
        [[nodiscard]] Optional<size_t> find_any_of(StringView needles, SearchDirection direction = SearchDirection::Forward) const
        {
            return StringUtils::find_any_of(*this, needles, direction);
        }

        /**
         * @param start 
         * @param length 
         * @return constexpr StringView 
         */
        [[nodiscard]] constexpr StringView substring_view(size_t start, size_t length) const
        {
            if(!is_constant_evaluated())
                VERIFY(start + length <= m_length);
            return {m_characters + start, length};
        }

        /**
         * @param start 
         * @return constexpr StringView 
         */
        [[nodiscard]] constexpr StringView substring_view(size_t start) const
        {
            if(!is_constant_evaluated())
                VERIFY(start <= length());
            return substring_view(start, length() - start);
        }

        /**
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(char, bool keep_empty = false) const;

        /**
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view(StringView, bool keep_empty = false) const;

        /**
         * @param predicate 
         * @param keep_empty 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> split_view_if(Function<bool(char)> const& predicate, bool keep_empty = false) const;

        /**
         * @tparam Callback 
         * @param separator 
         * @param keep_empty 
         * @param callback 
         */
        template <VoidFunction<StringView> Callback>
        void for_each_split_view(char separator, bool keep_empty, Callback callback) const
        {
            StringView seperator_view{&separator, 1};
            for_each_split_view(seperator_view, keep_empty, callback);
        }

        /**
         * @tparam Callback 
         * @param separator 
         * @param keep_empty 
         * @param callback 
         */
        template <VoidFunction<StringView> Callback>
        void for_each_split_view(StringView separator, bool keep_empty, Callback callback) const
        {
            VERIFY(!separator.is_empty());

            if(is_empty())
                return;

            StringView view{*this};

            auto maybe_separator_index = find(separator);
            while(maybe_separator_index.has_value())
            {
                auto separator_index = maybe_separator_index.value();
                auto part_with_separator = view.substring_view(0, separator_index + separator.length());
                if(keep_empty || separator_index > 0)
                    callback(part_with_separator.substring_view(0, separator_index));
                view = view.substring_view_starting_after_substring(part_with_separator);
                maybe_separator_index = view.find(separator);
            }
            if(keep_empty || !view.is_empty())
                callback(view);
        }

        /**
         * @param consider_cr 
         * @return Vector<StringView> 
         */
        [[nodiscard]] Vector<StringView> lines(bool consider_cr = true) const;

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template <typename T = int>
        Optional<T> to_int() const;
        template <typename T = unsigned>
        Optional<T> to_uint() const;

        /**
         * @param substring 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view_starting_from_substring(StringView substring) const;

        /**
         * @param substring 
         * @return StringView 
         */
        [[nodiscard]] StringView substring_view_starting_after_substring(StringView substring) const;

        /**
         * @param buffer 
         * @param buffer_size 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool copy_characters_to_buffer(char* buffer, size_t buffer_size) const;

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(char const* cstring) const
        {
            if(is_null())
                return cstring == nullptr;

            if(!cstring)
                return false;

            char const* cp = cstring;
            for(size_t i = 0; i < m_length; ++i)
            {
                if(*cp == '\0')
                    return false;
                if(m_characters[i] != *(cp++))
                    return false;
            }
            return *cp == '\0';
        }

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(char const* cstring) const
        {
            return !(*this == cstring);
        }

    #ifndef KERNEL
        bool operator==(String const&) const;
    #endif

        /**
         * @param other 
         * @return constexpr int 
         */
        [[nodiscard]] constexpr int compare(StringView other) const
        {
            if(m_characters == nullptr)
                return other.m_characters ? -1 : 0;

            if(other.m_characters == nullptr)
                return 1;

            size_t rlen = min(m_length, other.m_length);
            int c = __builtin_memcmp(m_characters, other.m_characters, rlen);
            if(c == 0)
            {
                if(length() < other.length())
                    return -1;
                if(length() == other.length())
                    return 0;
                return 1;
            }
            return c;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(StringView other) const
        {
            return length() == other.length() && compare(other) == 0;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(StringView other) const
        {
            return length() != other.length() || compare(other) != 0;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<(StringView other) const
        {
            return compare(other) < 0;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<=(StringView other) const
        {
            return compare(other) <= 0;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>(StringView other) const
        {
            return compare(other) > 0;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>=(StringView other) const
        {
            return compare(other) >= 0;
        }

    #ifndef KERNEL
        [[nodiscard]] String to_string() const;
    #endif

        [[nodiscard]] bool is_whitespace() const
        {
            return StringUtils::is_whitespace(*this);
        }

    #ifndef KERNEL
        [[nodiscard]] String replace(StringView needle, StringView replacement, bool all_occurrences = false) const;
    #endif
        [[nodiscard]] size_t count(StringView needle) const
        {
            return StringUtils::count(*this, needle);
        }
        
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
        friend class String;
        char const* m_characters{nullptr};
        size_t m_length{0};
    }; // class StringView

    template <>
    struct Traits<StringView> : public GenericTraits<StringView>
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(StringView s)
        {
            return s.hash();
        }
    }; // struct Traits<StringView> : public GenericTraits<StringView>

    struct CaseInsensitiveStringViewTraits : public Traits<StringView>
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(StringView s)
        {
            if(s.is_empty())
                return 0;
            return case_insensitive_string_hash(s.characters_without_null_termination(), s.length());
        }
    }; // struct CaseInsensitiveStringViewTraits : public Traits<StringView>
} // namespace Mods

#if defined(__clang__)
#define MODS_STRING_VIEW_LITERAL_CONSTEVAL constexpr
#else
#define MODS_STRING_VIEW_LITERAL_CONSTEVAL consteval
#endif

[[nodiscard]] ALWAYS_INLINE MODS_STRING_VIEW_LITERAL_CONSTEVAL Mods::StringView operator"" sv(char const* cstring, size_t length)
{
    return Mods::StringView(cstring, length);
}

using Mods::CaseInsensitiveStringViewTraits;
using Mods::StringView;