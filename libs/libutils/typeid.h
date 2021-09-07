#pragma once

// includes
#include <libutils/prelude.h>

namespace Utils
{

using TypeId = uinptr_t;

template <typename T>
static TypeId GetTypeId()
{
    static uint32_t placeHolder;
    return (reinterpret_cast<TypeId>(&placeHolder));
}

constexpr TypeId INVALID_TYPE_ID = 1;

}