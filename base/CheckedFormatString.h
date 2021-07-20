/*
 * Copyright (c) 2021, krishpranav, Andrew-stew
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/AllOf.h>
#include <base/AnyOf.h>
#include <base/StdLibExtras.h>
#include <base/StringView.h>

#ifdef ENABLE_COMPILETIME_FORMAT_CHECK
#    ifdef __clang__
#        undef ENABLE_COMPILETIME_FORMAT_CHECK
#    endif
#endif


#ifdef ENABLE_COMPILETIME_FORMAT_CHECK
namespace Base::Format::Detail {

template<typename T, size_t Size>
struct Array {
    constexpr static size_t size() { return Size; }
    constexpr const T& operator[](size_t index) const { return __data[index]; }
    constexpr T& operator[](size_t index) { return __data[index]; }
    using ConstIterator = SimpleIterator<const Array, const T>;
    using Iterator = SimpleIterator<Array, T>;

    constexpr ConstIterator begin() const { return ConstIterator::begin(*this); }
    constexpr Iterator begin() { return Iterator::begin(*this); }

    constexpr ConstIterator end() const { return ConstIterator::end(*this); }
    constexpr Iterator end() { return Iterator::end(*this); }

    T __data[Size];
};

template<typename... Args>
void compiletime_fail(Args...);

template<size_t N>
consteval auto extract_used_argument_index(const char (&fmt)[N], size_t specifier_start_index, size_t specifier_end_index, size_t& next_implicit_argument_index)
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

template<size_t N>
consteval auto count_fmt_params(const char (&fmt)[N])
{
    struct {
        // FIXME: Switch to variable-sized storage whenever we can come up with one :)
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

            // Note: There's no compile-time throw, so we have to abuse a compile-time string to store errors.
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

                const auto specifier_start_index = result.last_format_specifier_start[--result.total_used_last_format_specifier_start_count];

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
}

#endif

}