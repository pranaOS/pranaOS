#pragma once

#include <libutils/prelude.h>

namespace Utils
{

template <typename TValue>
constexpr TValue swap_little_endian(TValue value)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return value;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    if constexpr (sizeof(TValue) == 8)
        return __builtin_bswap64(value);
    if constexpr (sizeof(TValue) == 4)
        return __builtin_bswap32(value);
    if constexpr (sizeof(TValue) == 2)
        return __builtin_bswap16(value);
    if constexpr (sizeof(TValue) == 1)
        return value;
#endif
}

template <typename TValue>
constexpr TValue swap_big_endian(TValue value)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    if constexpr (sizeof(TValue) == 8)
        return __builtin_bswap64(value);
    if constexpr (sizeof(TValue) == 4)
        return __builtin_bswap32(value);
    if constexpr (sizeof(TValue) == 2)
        return __builtin_bswap16(value);
    if constexpr (sizeof(TValue) == 1)
        return value;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return value;
#endif
}

template <typename TValue>
struct PACKED BigEndian
{
private:
    TValue _value{0};

public:
    constexpr BigEndian() {}

    constexpr BigEndian(TValue value) : _value(swap_big_endian(value)) {}

    constexpr TValue operator()() const
    {
        return swap_big_endian(_value);
    }
};

}