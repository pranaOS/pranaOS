/**
 * @file hmac.h
 * @author Krisna Pranav
 * @brief hmac
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
constexpr static auto OPAD = 0x5c;

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

            /**
             * @param message 
             * @param length 
             * @return TagType 
             */
            TagType process(const u8* message, size_t length)
            {
                reset();
                update(message, length);
                return digest();
            }

            /**
             * @param message 
             * @param length 
             */
            void update(const u8* message, size_t length)
            {
                m_inner_hasher.update(message, length);
            }

            /**
             * @param span 
             * @return TagType 
             */
            TagType process(const ReadonlyBytes& span) 
            { 
                return process(span.data(), span.size()); 
            }

            /**
             * @param buffer 
             * @return TagType 
             */
            TagType process(const ByteBuffer& buffer) 
            { 
                return process(buffer.data(), buffer.size()); 
            }

            /**
             * @param string 
             * @return TagType 
             */
            TagType process(const StringView& string) 
            { 
                return process((const u8*)string.characters_without_null_termination(), string.length()); 
            }

            /**
             * @param span 
             */
            void update(const ReadonlyBytes& span) 
            { 
                return update(span.data(), span.size()); 
            }

            /**
             * @param buffer 
             */
            void update(const ByteBuffer& buffer) 
            { 
                return update(buffer.data(), buffer.size()); 
            }

            /**
             * @param string 
             */
            void update(const StringView& string) 
            { 
                return update((const u8*)string.characters_without_null_termination(), string.length()); 
            }

            /**
             * @return TagType 
             */
            TagType digest()
            {
                m_outer_hasher.update(m_inner_hasher.digest().immutable_data(), m_inner_hasher.digest_size());
                auto result = m_outer_hasher.digest();
                reset();
                return result;
            }

            void reset()
            {
                m_inner_hasher.reset();
                m_outer_hasher.reset();
                m_inner_hasher.update(m_key_data, m_inner_hasher.block_size());
                m_outer_hasher.update(m_key_data + m_inner_hasher.block_size(), m_outer_hasher.block_size());
            }

            /**
             * @return String 
             */
            String class_name() const
            {
                StringBuilder builder;
                builder.append("HMAC-");
                builder.append(m_inner_hasher.class_name());
                return builder.build();
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
                    key_buffer.overwrite(0, digest.immutable_data(), m_inner_hasher.digest_size());
                } else {
                    key_buffer.overwrite(0, key, length);
                }

                auto* i_key = m_key_data;
                auto* o_key = m_key_data + block_size;

                for (size_t i = 0; i < block_size; ++i) {
                    auto key_byte = key_buffer[i];
                    i_key[i] = key_byte ^ IPAD;
                    o_key[i] = key_byte ^ OPAD;
                }
            }

            /**
             * @param key 
             */
            void derive_key(const ByteBuffer& key) 
            { 
                derive_key(key.data(), key.size()); 
            }

            /**
             * @param key 
             */
            void derive_key(const StringView& key) 
            { 
                derive_key((const u8*)key.characters_without_null_termination(), key.length()); 
            }

            HashType m_inner_hasher, m_outer_hasher;
            u8 m_key_data[2048];
        }; // class HMAC
    } // namespace Authentication
} // namespace Crypto