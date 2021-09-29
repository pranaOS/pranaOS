/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/stdextras.h>
#include <libutils/types.h>

namespace Utils {

struct ByteReader {

    template<typename T>
    static T* load_pointer(const u8* address)
    {
        FlatPtr value;
        load<FlatPtr>(address, value);

        return reinterpret_cast<T*>(value);
    }

    static u16 load16(const u8* address)
    {
        u16 value;
        load(address, value);
        return value;
    }

    static u32 load32(const u8* address)
    {  
        u32 value;
        load(address, value);

        return value;
    }

    static u64 load64(const u8* address)
    {
        u64 value;
        load(address, value);

        return value;
    }
    
};

}

using Utils::ByteReader;