#pragma once

#include <libutils/prelude.h>

namespace Text
{

typedef uint32_t Rune;

inline bool rune_is_digit(Rune rune)
{
    return (rune >= U'0' && rune <= U'9');
}

inline int rune_numeric_value(Rune rune)
{
    if (rune_is_digit(rune))
    {
        return rune - U'0';
    }
    else
    {
        return 0;
    }
}

template <typename T>
inline void rune_foreach(const uint8_t *buffer, T callback)
{
    Rune rune = 0;

    size_t size = utf8_to_rune(buffer, &rune);
    buffer += size;

    while (size && rune != 0)
    {
        callback(rune);

        size = utf8_to_rune(buffer, &rune);
        buffer += size;
    }
}

}