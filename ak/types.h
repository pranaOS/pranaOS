/*
 * Copyright (c) 2020 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

namespace ak {
    typedef char                        int8_t;
    typedef unsigned char               uint8_t;
    typedef short                       int16_t;
    typedef unsigned short              uint16_t;
    typedef int                         int32_t;
    typedef unsigned int                uint32_t;
    typedef long long int               int64_t;
    typedef unsigned long long int      uint64_t;
    typedef unsigned long long          uintptr_t;

    constexpr uint64_t divide64(uint64_t n, uint32_t base, uint32_t* r = 0) {
        uint64_t rem = n;
        uint64_t b = base;
        uint64_t res = 0, d = 1;
        uint32_t high = rem >> 32;

        if (high >= base) {
                high /= base;
                res = (uint64_t) high << 32;
                rem -= (uint64_t) (high*base) << 32;
        }

        while ((int64_t)b > 0 && b < rem) {
                b = b+b;
                d = d+d;
        }

        do {
            if (rem >= b) {
                rem -= b;
                res += d;
            }

            b >>= 1;
            d >>= 1;

            } while (d);

            if(r)
                *r = rem;
            
            return res;
        }
    }

    constexpr ak::uint32_t operator"" _KB(unsigned long long no) {
        return no * 1024;
    }

    constexpr ak::uint32_t operator"" _MB(unsigned long long no {
        return no * (1024_KB);
    }
    
    constexpr ak::uint32_t operator"" _GB(unsigned long long no) {
        return no * (1024_MB);
    }
}
