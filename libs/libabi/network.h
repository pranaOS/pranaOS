#pragma once

#include <libutils/prelude.h>

struct Address
{
    uint8_t operator[](int index) const
    {
        return bytes[index];
    }
};