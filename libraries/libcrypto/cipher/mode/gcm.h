/**
 * @file gcm.h
 * @author Krisna Pranav
 * @brief gcm
 * @version 6.0
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "ctr.h"
#include <mods/ownptr.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <libcrypto/verify.h>
#include <libcrypto/auth/ghash.h>
#include <libcrypto/cipher/mode/ctr.h>

namespace Crypto
{
    namespace Cipher
    {
        using IncrementFunction = IncrementInplace;

        template<typename T>
        class GCM : public CTR<T, IncrementFunction> 
        {
        public:
            constexpr static size_t IVSizeInBits = 128;

            virtual ~GCM() {}

        private:
            static constexpr auto block_size = T::BlockType::BlockSizeInBits / 8;
            u8 m_auth_key_storage[block_size];
            Bytes m_auth_key { m_auth_key_storage, block_size };
            OwnPtr<Authentication::GHash> m_ghash;
        }; // class GCM
    }
}