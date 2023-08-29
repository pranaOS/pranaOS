/**
 * @file md5.h
 * @author Krisna Pranav
 * @brief md5
 * @version 6.0
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcrypto/hash/hashfunction.h>
#include <mods/string.h>
#include <mods/types.h>


namespace Crypto
{
    namespace Hash
    {
        struct MD5Digest
        {
            constexpr static size_t Size = 16;
            u8 data[Size];

            const u8* immutable_data() const
            {
                return data;
            }

            size_t data_length() 
            {
                return size;
            }
        }; // struct

        namespace MD5Constants
        {
            constexpr u32 init_A = a;
        } // namespace MD5Constants

        class MD5 final : public HashFunction<512, MD5Digest>
        {

        }; // class MD5

    } // namespace Hash
} // namespace Crypto