/**
 * @file checkformatstring.h
 * @author Krisna Pranav
 * @brief Check Format String
 * @version 6.0
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/allof.h>
#include <mods/anyof.h>
#include <mods/array.h>
#include <mods/stringview.h>

#ifdef ENABLE_COMPILETIME_FORMAT_CHECK
namespace Mods::Format::Detail 
{

    /**
     * @tparam T 
     * @tparam Size 
     */
    template<typename T, size_t Size>
    struct Array 
    {
        /**
         * @return constexpr size_t 
         */
        constexpr static size_t size() 
        { 
            return Size; 
        }

        /**
         * @param index 
         * @return constexpr T const& 
         */
        constexpr T const& operator[](size_t index) const 
        { 
            return __data[index]; 
        }

        /**
         * @param index 
         * @return constexpr T& 
         */
        constexpr T& operator[](size_t index) 
        { 
            return __data[index]; 
        }

        using ConstIterator = SimpleIterator<Array const, T const>;
        using Iterator = SimpleIterator<Array, T>;

        /**
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }

        constexpr Iterator begin() 
        { 
            return Iterator::begin(*this); 
        }

        constexpr ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        constexpr Iterator end() 
        { 
            return Iterator::end(*this); 
        }

        T __data[Size];
    }; // struct Array 

    /**
     * @tparam N 
     * @param specifier_start_index 
     * @param specifier_end_index 
     * @param next_implicit_argument_index 
     * @return consteval 
     */
    template<size_t N>
    consteval auto extract_used_argument_index(char const (&fmt)[N], size_t specifier_start_index, size_t specifier_end_index, size_t& next_implicit_argument_index)
    {
        struct {
            size_t index_value { 0 };
            bool saw_explicit_index { false };
        } state;
        for (size_t i = specifier_start_index; i < specifier_end_index; ++i) {
            auto c = fmt[i];
            if (c > '9' || c < '0')
                break;

            state.index_value *= 10;
            state.index_value += c - '0';
            state.saw_explicit_index = true;
        }

        if (!state.saw_explicit_index)
            return next_implicit_argument_index++;

        return state.index_value;
    }

    /**
     * @tparam N 
     * @return consteval 
     */
    template<size_t N>
    consteval auto count_fmt_params(char const (&fmt)[N])
    {
        struct {
            Array<size_t, 128> used_arguments { 0 };
            size_t total_used_argument_count { 0 };
            size_t next_implicit_argument_index { 0 };
            bool has_explicit_argument_references { false };

            size_t unclosed_braces { 0 };
            size_t extra_closed_braces { 0 };
            size_t nesting_level { 0 };

            Array<size_t, 4> last_format_specifier_start { 0 };
            size_t total_used_last_format_specifier_start_count { 0 };
        } result;

        for (size_t i = 0; i < N; ++i) {
            auto ch = fmt[i];
            switch (ch) {
            case '{':
                if (i + 1 < N && fmt[i + 1] == '{') {
                    ++i;
                    continue;
                }

                if (result.total_used_last_format_specifier_start_count >= result.last_format_specifier_start.size() - 1)
                    compiletime_fail("Format-String Checker internal error: Format specifier nested too deep");

                result.last_format_specifier_start[result.total_used_last_format_specifier_start_count++] = i + 1;

                ++result.unclosed_braces;
                ++result.nesting_level;
                break;
            case '}':
                if (result.nesting_level == 0) {
                    if (i + 1 < N && fmt[i + 1] == '}') {
                        ++i;
                        continue;
                    }
                }
                if (result.unclosed_braces) {
                    --result.nesting_level;
                    --result.unclosed_braces;

                    if (result.total_used_last_format_specifier_start_count == 0)
                        compiletime_fail("Format-String Checker internal error: Expected location information");

                    auto const specifier_start_index = result.last_format_specifier_start[--result.total_used_last_format_specifier_start_count];

                    if (result.total_used_argument_count >= result.used_arguments.size())
                        compiletime_fail("Format-String Checker internal error: Too many format arguments in format string");

                    auto used_argument_index = extract_used_argument_index<N>(fmt, specifier_start_index, i, result.next_implicit_argument_index);
                    if (used_argument_index + 1 != result.next_implicit_argument_index)
                        result.has_explicit_argument_references = true;
                    result.used_arguments[result.total_used_argument_count++] = used_argument_index;

                } else {
                    ++result.extra_closed_braces;
                }
                break;
            default:
                continue;
            }
        }
        return result;
    }
} // namespace Mods::Format::Detail 

#endif

namespace Mods::Format::Detail 
{
    /**
     * @tparam Args 
     */
    template<typename... Args>
    struct CheckedFormatString {
        /**
         * @tparam N 
         * @return consteval 
         */
        template<size_t N>
        consteval CheckedFormatString(char const (&fmt)[N])
            : m_string { fmt, N - 1 }
        {
    #ifdef ENABLE_COMPILETIME_FORMAT_CHECK
            check_format_parameter_consistency<N, sizeof...(Args)>(fmt);
    #endif
        }

        /**
         * @tparam T 
         */
        template<typename T>
        CheckedFormatString(T const& unchecked_fmt)
        requires(requires(T t) { StringView { t }; })
            : m_string(unchecked_fmt)
        {
        }

        auto view() const 
        { 
            return m_string; 
        }

    private:
    #ifdef ENABLE_COMPILETIME_FORMAT_CHECK
        template<size_t N, size_t param_count>
        consteval static bool check_format_parameter_consistency(char const (&fmt)[N])
        {
            auto check = count_fmt_params<N>(fmt);
            if (check.unclosed_braces != 0)
                compiletime_fail("Extra unclosed braces in format string");
            if (check.extra_closed_braces != 0)
                compiletime_fail("Extra closing braces in format string");

            {
                auto begin = check.used_arguments.begin();
                auto end = check.used_arguments.begin() + check.total_used_argument_count;
                auto has_all_referenced_arguments = !Mods::any_of(begin, end, [](auto& entry) { return entry >= param_count; });
                if (!has_all_referenced_arguments)
                    compiletime_fail("Format string references nonexistent parameter");
            }

            if (!check.has_explicit_argument_references && check.total_used_argument_count != param_count)
                compiletime_fail("Format string does not reference all passed parameters");

            if (check.has_explicit_argument_references) {
                auto all_parameters = iota_array<size_t, param_count>(0);
                constexpr auto contains = [](auto begin, auto end, auto entry) {
                    for (; begin != end; begin++) {
                        if (*begin == entry)
                            return true;
                    }

                    return false;
                };
                auto references_all_arguments = Mods::all_of(
                    all_parameters,
                    [&](auto& entry) {
                        return contains(
                            check.used_arguments.begin(),
                            check.used_arguments.begin() + check.total_used_argument_count,
                            entry);
                    });
                if (!references_all_arguments)
                    compiletime_fail("Format string does not reference all passed parameters");
            }

            return true;
        }
    #endif

        StringView m_string;
    };
} // namespace Mods::Format::Detail

namespace Mods 
{

    template<typename... Args>
    using CheckedFormatString = Format::Detail::CheckedFormatString<IdentityType<Args>...>;

} // namespace Mods
