/**
 * @file hmac.h
 * @author Krisna Pranav
 * @brief hmac
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <mods/types.h>

constexpr static auto IPAD = 0x36;
constexpr static OPAD = 0x5c;

namespace Crypto
{
    namespace Authentication
    {

        template<typename HashT>
        class HMAC
        {
        public:
            using HashType = HashT;
            using TagType = typename HashType::DigestType;

            /**
             * @return size_t 
             */
            size_t digest_size() const
            {
                return m_inner_hasher.digest_size();
            }

            /**
             * @tparam KeyBufferType 
             * @tparam Args 
             * @param key 
             * @param args 
             */
            template<typename KeyBufferType, typename... Args>
            HMAC(KeyBufferType key, Args... args)
                : m_inner_hasher(args...)
                , m_outer_hasher(args...)
            {
                derive_key(key);
                reset();
            }

        private:
            /**
             * @param key 
             * @param length 
             */
            void derive_key(const u8* key, size_t length)
            {
                auto block_size = m_inner_hasher.block_size();
                u8 v_key[block_size];
                __builtin_memset(v_key, 0, block_size);
                ByteBuffer key_buffer = ByteBuffer::wrap(v_key, block_size);

                if (length > block_size) {
                    m_inner_hasher.update(key, length);
                    auto digest = m_inner_hasher.digest();
                    key_buffer.overwrite(0, digest.immutable_data());
                } else {
                    key_buffer.overwrite(0, key, length);
                }
            }

            /**
             * @param key 
             */
            void derive_key(const ByteBuffer& key)
            {
                derive_key(key.data(), key.size());
            }

            HashType m_inner_hasher, m_outer_hasher;
            u8 m_key_data[2048];
        }; // class HMAC

    }
}