/**
 * @file cbc.h
 * @author Krisna Pranav
 * @brief cbc
 * @version 6.0
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <libcrypto/cipher/mode/mode.h>

namespace Crypto 
{
    namespace Cipher 
    {

        template<typename T>
        class CBC : public Mode<T> 
        {
        public:
            constexpr static size_t IVSizeInBits = 128;
            
            /// @brief Destroy the CBC object
            virtual ~CBC() { }

            /**
             * @tparam Args 
             */
            template<typename... Args>
            explicit constexpr CBC<T>(Args... args)
                : Mode<T>(args...)
            {
            }

            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                StringBuilder builder;

                builder.append(this->cipher().class_name());
                builder.append("_CBC");

                return builder.build();
            }

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
            virtual void encrypt(const ReadonlyBytes& in, Bytes& out, const Bytes& ivec = {}, Bytes* ivec_out = nullptr) override
            {
                auto length = in.size();

                if (length == 0)
                    return;

                auto& cipher = this->cipher();

                ASSERT(!ivec.is_empty());
                const auto* iv = ivec.data();

                m_cipher_block.set_padding_mode(cipher.padding_mode());
                size_t offset { 0 };
                auto block_size = cipher.block_size();

                while (length >= block_size) {
                    m_cipher_block.overwrite(in.slice(offset, block_size));
                    m_cipher_block.apply_initialization_vector(iv);
                    cipher.encrypt_block(m_cipher_block, m_cipher_block);
                    ASSERT(offset + block_size <= out.size());
                    __builtin_memcpy(out.offset(offset), m_cipher_block.get().data(), block_size);
                    iv = out.offset(offset);
                    length -= block_size;
                    offset += block_size;
                }

                if (length > 0) {
                    m_cipher_block.overwrite(in.slice(offset, length));
                    m_cipher_block.apply_initialization_vector(iv);
                    cipher.encrypt_block(m_cipher_block, m_cipher_block);
                    ASSERT(offset + block_size <= out.size());
                    __builtin_memcpy(out.offset(offset), m_cipher_block.get().data(), block_size);
                    iv = out.offset(offset);
                }

                if (ivec_out)
                    __builtin_memcpy(ivec_out->data(), iv, min(IV_length(), ivec_out->size()));
            }

            /**
             * @param in 
             * @param out 
             * @param ivec 
             */
            virtual void decrypt(const ReadonlyBytes& in, Bytes& out, const Bytes& ivec = {}) override
            {
                auto length = in.size();

                if (length == 0)
                    return;

                auto& cipher = this->cipher();

                ASSERT(!ivec.is_empty());
                const auto* iv = ivec.data();

                auto block_size = cipher.block_size();

                ASSERT(length % block_size == 0);

                m_cipher_block.set_padding_mode(cipher.padding_mode());
                size_t offset { 0 };

                while (length > 0) {
                    auto* slice = in.offset(offset);
                    m_cipher_block.overwrite(slice, block_size);
                    cipher.decrypt_block(m_cipher_block, m_cipher_block);
                    m_cipher_block.apply_initialization_vector(iv);
                    auto decrypted = m_cipher_block.get();
                    ASSERT(offset + decrypted.size() <= out.size());
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
        }; // class CBC
    } // namespace Cipher
} // namespace Crypto