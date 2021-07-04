/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/Prelude.h>

namespace Utils
{

using TypeId = uintptr_t;

template <typename T>
static TypeId GetTypeId()
{
    static uint32_t placeHolder;
    return (reinterpret_cast<TypeId>(&placeHolder));
}


}