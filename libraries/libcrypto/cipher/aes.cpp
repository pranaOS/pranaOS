/**
 * @file aes.cpp
 * @author Krisna Pranav
 * @brief aes
 * @version 6.0
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/stringbuilder.h>
#include <libcrypto/cipher/aes.h>
#include <libcrypto/cipher/aestables.h>

namespace Crypto
{

    namespace Cipher
    {

        template<typename T>
        constexpr u32 get_key(T pt)
        {
        }

        constexpr void swap_keys(u32* keys, size_t i, size_t j)
        {
            u32 temp = keys[i];
            keys[i] = keys[j];
            keys[j] = temp;
        }

    } // namespace Cipher

} // namespace Crypto