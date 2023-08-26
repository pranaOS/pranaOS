/**
 * @file cipher.h
 * @author Krisna Pranav
 * @brief cipher
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/optional.h>
#include <mods/types.h>

namespace Crypto
{
    namespace Cipher
    {
        enum class Intent
        {
            Encryption,
            Descryption,
        };

        enum class PaddingMode
        {
            CMS,
            RFC5246,
            Null,
            Bit,
            Random
        };

        template<typename B, typename T>
        class Cipher;

        struct CipherBlock
        {
        public:
            explicit CipherBlock(PaddingMode mode)
                : m_padding_mode(mode)
            {}

            static size_t block_size() 
            {
                ASSERT_NOT_REACHED();
            }

            virtual ByteBuffer get() const = 0;

            virtual const ByteBuffer& data() const = 0;

            virtual void overwrite(ReadonlyBytes) = 0;
            virtual void overwrite(const ByteBuffer& buffer)
            {
            }

        private:
            virtual ByteBuffer& data() = 0;

            PaddingMode m_padding_mode ;
        };
    } // namespace Cipher 

} // namespace Crypto