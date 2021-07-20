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

}