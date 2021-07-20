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
};

}