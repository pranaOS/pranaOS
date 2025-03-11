/**
 * @file hmac.h
 * @author Krisna Pranav
 * @brief hmac
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>
#include <mods/types.h>
#include <mods/vector.h>

#ifndef KERNEL
#    include <mods/string.h>
#endif

constexpr static auto IPAD = 0x36;
constexpr static auto OPAD = 0x5c;

namespace Crypto 
{
    namespace Authentication 
    {

        /**
         * @tparam HashT 
         */
        template<typename HashT>
        class HMAC 
        {
        public:
            using HashType = HashT;
            using TagType = typename HashType::DigestType;

            /**
             * @return constexpr size_t 
             */
            constexpr size_t digest_size() const 
            { 
                return m_inner_hasher.digest_size(); 
            }

            /**
             * @brief Construct a new HMAC object
             * 
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
            TagType process(u8 const* message, size_t length)
            {
                reset();
                update(message, length);
                return digest();
            }

            /**
             * @param message 
             * @param length 
             */
            void update(u8 const* message, size_t length)
            {
                m_inner_hasher.update(message, length);
            }

            /**
             * @param span 
             * @return TagType 
             */
            TagType process(ReadonlyBytes span) 
            { 
                return process(span.data(), span.size()); 
            }

            /**
             * @param string 
             * @return TagType 
             */
            TagType process(StringView string) 
            { 
                return process((u8 const*)string.characters_without_null_termination(), string.length()); 
            }

            /**
             * @param span 
             */
            void update(ReadonlyBytes span) 
            { 
                return update(span.data(), span.size()); 
            }

            /**
             * @param string 
             */
            void update(StringView string) 
            { 
                return update((u8 const*)string.characters_without_null_termination(), string.length()); 
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

        #ifndef KERNEL
            String class_name() const
            {
                StringBuilder builder;
                builder.append("HMAC-");
                builder.append(m_inner_hasher.class_name());
                return builder.build();
            }
        #endif

        private:
            /**
             * @param key 
             * @param length 
             */
            void derive_key(u8 const* key, size_t length)
            {
                auto block_size = m_inner_hasher.block_size();
                
                Vector<u8, 64> v_key;
                v_key.resize(block_size);
                auto key_buffer = v_key.span();
                
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
            void derive_key(ReadonlyBytes key) 
            { 
                derive_key(key.data(), key.size()); 
            }

            /**
             * @param key 
             */
            void derive_key(StringView key) 
            { 
                derive_key(key.bytes()); 
            }

            HashType m_inner_hasher, m_outer_hasher;
            u8 m_key_data[2048];
        }; // class HMAC

    } // namespace Authentication

} // namespace Crypto
