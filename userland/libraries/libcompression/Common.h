/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Result.h>
#include <libutils/Array.h>
#include <libutils/Prelude.h>

namespace Compression
{

struct CRCTable : public Array<uint32_t, 256>
{
    static constexpr uint32_t POLYNOMIAL = 0xEDB88320;

    constexpr CRCTable()
    {
        uint32_t remainder = 0;
        uint8_t b = 0;

        do
        {
            remainder = b;

            for (uint8_t bit = 8; bit > 0; --bit)
            {
                if (remainder & 1)
                {
                    remainder = (remainder >> 1) ^ POLYNOMIAL;
                }
                else
                {
                    remainder = (remainder >> 1);
                }
            }

            at(b) = remainder;
        } while (0 != ++b);
    }
};

}