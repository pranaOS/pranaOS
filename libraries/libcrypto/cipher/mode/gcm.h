/**
 * @file gcm.h
 * @author Krisna Pranav
 * @brief gcm
 * @version 6.0
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

            /// @brief Destroy the GCM object
            virtual ~GCM() { }

            /**
             * @tparam Args 
             */
            template<typename... Args>
            explicit constexpr GCM<T>(Args... args)
                : CTR<T>(args...)
            {
                static_assert(T::BlockSizeInBits == 128u, "GCM Mode is only available for 128-bit Ciphers");

                __builtin_memset(m_auth_key_storage, 0, block_size);

                typename T::BlockType key_block(m_auth_key_storage, block_size);

                this->cipher().encrypt_block(key_block, key_block);
                key_block.bytes().copy_to(m_auth_key);

                m_ghash = make<Authentication::GHash>(m_auth_key);
            }

            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                StringBuilder builder;
                builder.append(this->cipher().class_name());
                builder.append("_GCM");

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
             */
            virtual void encrypt(const ReadonlyBytes& in, Bytes& out, const Bytes& ivec = {}, Bytes* = nullptr) override
            {
                ASSERT(!ivec.is_empty());

                static ByteBuffer dummy;

                encrypt(in, out, ivec, dummy, dummy);
            }

            /**
             * @param in 
             * @param out 
             * @param ivec 
             */
            virtual void decrypt(const ReadonlyBytes& in, Bytes& out, const Bytes& ivec = {}) override
            {
                encrypt(in, out, ivec);
            }

            /**
             * @param in 
             * @param out 
             * @param iv_in 
             * @param aad 
             * @param tag 
             */
            void encrypt(const ReadonlyBytes& in, Bytes out, const ReadonlyBytes& iv_in, const ReadonlyBytes& aad, Bytes tag)
            {
                auto iv_buf = ByteBuffer::copy(iv_in.data(), iv_in.size());
                auto iv = iv_buf.bytes();

                CTR<T>::increment(iv);

                typename T::BlockType block0;

                block0.overwrite(iv);
                this->cipher().encrypt_block(block0, block0);

                CTR<T>::increment(iv);

                if (in.is_empty())
                    CTR<T>::key_stream(out, iv);
                else
                    CTR<T>::encrypt(in, out, iv);

                auto auth_tag = m_ghash->process(aad, out);
                block0.apply_initialization_vector(auth_tag.data);
                block0.get().bytes().copy_to(tag);
            }

            /**
             * @param in 
             * @param out 
             * @param iv_in 
             * @param aad 
             * @param tag 
             * @return VerificationConsistency 
             */
            VerificationConsistency decrypt(const ReadonlyBytes& in, Bytes out, const ReadonlyBytes& iv_in, const ReadonlyBytes& aad, const ReadonlyBytes& tag)
            {
                auto iv_buf = ByteBuffer::copy(iv_in.data(), iv_in.size());
                auto iv = iv_buf.bytes();

                CTR<T>::increment(iv);
                typename T::BlockType block0;
                block0.overwrite(iv);
                this->cipher().encrypt_block(block0, block0);

                CTR<T>::increment(iv);

                auto auth_tag = m_ghash->process(aad, in);
                block0.apply_initialization_vector(auth_tag.data);

                auto test_consistency = [&] {
                    if (block0.block_size() != tag.size() || __builtin_memcmp(block0.bytes().data(), tag.data(), tag.size()) != 0)
                        return VerificationConsistency::Inconsistent;

                    return VerificationConsistency::Consistent;
                };

                if (in.is_empty()) {
                    out = {};
                    return test_consistency();
                }

                CTR<T>::encrypt(in, out, iv);
                return test_consistency();
            }

        private:
            static constexpr auto block_size = T::BlockType::BlockSizeInBits / 8;
            u8 m_auth_key_storage[block_size];
            Bytes m_auth_key { m_auth_key_storage, block_size };
            OwnPtr<Authentication::GHash> m_ghash;
        }; // class GCM
    } // namespace Cipher
} // namespace Crypto
