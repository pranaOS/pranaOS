/**
 * @file cbc.h
 * @author Krisna Pranav
 * @brief cbc
 * @version 6.0
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stringbuilder.h>
#include <mods/stringview.h>
#include <libcrypto/cipher/mode/mode.h>

#ifndef KERNEL
#    include <mods/string.h>
#endif

namespace Crypto 
{

    namespace Cipher 
    {

        /**
         * @tparam T 
         */
        template<typename T>
        class CBC : public Mode<T> 
        {
        public:
            constexpr static size_t IVSizeInBits = 128;

            virtual ~CBC() = default;

            /**
             * @tparam Args 
             */
            template<typename... Args>
            explicit constexpr CBC<T>(Args... args)
                : Mode<T>(args...)
            {
            }

        #ifndef KERNEL
            virtual String class_name() const override
            {
                StringBuilder builder;
                builder.append(this->cipher().class_name());
                builder.append("_CBC");
                return builder.build();
            }
        #endif

            /**
             * @return size_t 
             */
            virtual size_t IV_length() const override
            {
                return IVSizeInBits / 8;
            }

            /**
             * @param in 
             * @param out 
             * @param ivec 
             * @param ivec_out 
             */
            virtual void encrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}, Bytes* ivec_out = nullptr) override
            {
                auto length = in.size();
                if (length == 0)
                    return;

                auto& cipher = this->cipher();

                VERIFY(!ivec.is_empty());
                ReadonlyBytes iv = ivec;

                m_cipher_block.set_padding_mode(cipher.padding_mode());
                size_t offset { 0 };
                auto block_size = cipher.block_size();

                while (length >= block_size) {
                    m_cipher_block.overwrite(in.slice(offset, block_size));
                    m_cipher_block.apply_initialization_vector(iv);

                    cipher.encrypt_block(m_cipher_block, m_cipher_block);
                    VERIFY(offset + block_size <= out.size());

                    __builtin_memcpy(out.offset(offset), m_cipher_block.bytes().data(), block_size);

                    iv = out.slice(offset);
                    length -= block_size;
                    offset += block_size;
                }

                if (length > 0) {
                    m_cipher_block.overwrite(in.slice(offset, length));
                    m_cipher_block.apply_initialization_vector(iv);
                    cipher.encrypt_block(m_cipher_block, m_cipher_block);

                    VERIFY(offset + block_size <= out.size());

                    __builtin_memcpy(out.offset(offset), m_cipher_block.bytes().data(), block_size);

                    iv = out.slice(offset);
                }

                if (ivec_out)
                    __builtin_memcpy(ivec_out->data(), iv.data(), min(IV_length(), ivec_out->size()));
            }

            /**
             * @param in 
             * @param out 
             * @param ivec 
             */
            virtual void decrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}) override
            {
                auto length = in.size();
                if (length == 0)
                    return;

                auto& cipher = this->cipher();

                VERIFY(!ivec.is_empty());
                ReadonlyBytes iv = ivec;

                auto block_size = cipher.block_size();

                VERIFY(length % block_size == 0);

                m_cipher_block.set_padding_mode(cipher.padding_mode());
                size_t offset { 0 };

                while (length > 0) {
                    auto slice = in.slice(offset);
                    m_cipher_block.overwrite(slice.data(), block_size);
                    cipher.decrypt_block(m_cipher_block, m_cipher_block);
                    m_cipher_block.apply_initialization_vector(iv);

                    auto decrypted = m_cipher_block.bytes();
                    VERIFY(offset + decrypted.size() <= out.size());

                    __builtin_memcpy(out.offset(offset), decrypted.data(), decrypted.size());

                    iv = slice;
                    length -= block_size;
                    offset += block_size;
                }
                out = out.slice(0, offset);
                this->prune_padding(out);
            }

        private:
            typename T::BlockType m_cipher_block {};
        }; // class CBC : public Mode<T> 

    } // namespace Cipher

} // namespace Crypto
