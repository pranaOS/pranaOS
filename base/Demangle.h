/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/String.h>
#include <base/StringView.h>

#ifndef BUILDING_PRANAOS_TOOLCHAIN
#    include <cxxabi.h>
#endif

namespace Base {

inline String demangle(const StringView& name)
{
#ifdef BUILDING_PRANAOS_TOOLCHAIN
    return name;
#else
    int status = 0;
    auto* demangled_name = abi::__cxa_demangle(name.to_string().characters(), nullptr, nullptr, &status);
    auto string = String(status == 0 ? demangled_name : name);
    if (status == 0)
        kfree(demangled_name);
    return string;
#endif
}

}

using Base::demangle;
