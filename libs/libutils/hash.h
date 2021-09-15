#pragma once

#include <libutils/prelude.h>

namespace Utils
{

static inline uint32_t hash(const void *object, size_t size)
{
    uint32_t hash = 5381;
}

template <typename TObject>
constexpr bool has_hash_function()
{
    return false;
}

}