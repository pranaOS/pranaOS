/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/CheckedFormatString.h>

#include <base/AllOf.h>
#include <base/AnyOf.h>
#include <base/Array.h>
#include <base/GenericLexer.h>
#include <base/Optional.h>
#include <base/StringView.h>

#ifndef KERNEL
#    include <stdio.h>
#endif

namespace Base {

class TypeErasedFormatParams;
class FormatParser;
class FormatBuilder;

template<typename T, typename = void>
struct Formatter {
    using __no_formatter_defined = void;
};

template<typename T, typename = void>
inline constexpr bool HasFormatter = true;

template<typename T>
inline constexpr bool HasFormatter<T, typename Formatter<T>::__no_formatter_defined> = false;

constexpr size_t max_format_arguments = 256;

}