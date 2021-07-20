/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/StringView.h>

#ifndef BUILDING_PRANAOS_TOOLCHAIN
#    include <cxxabi.h>
#endif


namespace Base {

inline String demange(const StringView& name)
{
#ifdef BUILDING_PRANAOS_TOOLCHAIN
    return name;
}

}