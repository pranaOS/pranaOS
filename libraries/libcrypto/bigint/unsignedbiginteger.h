/**
 * @file unsignedbiginteger.h
 * @author Krisna Pranav
 * @brief unsigned big integer
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/logstream.h>
#include <mods/span.h>
#include <mods/types.h>
#include <mods/string.h>
#include <mods/vector.h>

namespace Crypto
{
    struct UnsignedDivisionResult;

    constexpr size_t STARTING_WORD_SIZE = 512;

    class UnsignedBigInteger
    {
        UnsignedBigInteger(u32 x) 
        {

        }
    private:
        
    }; /// class
}