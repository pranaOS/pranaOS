/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libutils/Prelude.h>

struct MacAddress
{
    uint8_t bytes[6];

    uint8_t operator[](int index) const
    {
        return bytes[index];
    }
};