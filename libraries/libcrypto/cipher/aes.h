/**
 * @file aes.h
 * @author Krisna Pranav
 * @brief aes
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/vector.h>
#include <libcrypto/cipher/cipher.h>
#include <libcrypto/cipher/mode/cbc.h>
#include <libcrypto/cipher/mode/ctr.h>
#include <libcrypto/cipher/mode/gcm.h>

#ifndef KERNEL
#    include <mods/string.h>
#endif

namespace Crypto 
{
    namespace Cipher 
    {

        struct AESCipherBlock : public CipherBlock 
        {
        public:
            static constexpr size_t BlockSizeInBits = 128;

            /**
             * @brief Construct a new AESCipherBlock object
             * 
             * @param mode 
             */
            explicit AESCipherBlock(PaddingMode mode = PaddingMode::CMS)
                : CipherBlock(mode)
            {
            }

            /**
             * @brief Construct a new AESCipherBlock object
             * 
             * @param data 
             * @param length 
             * @param mode 
             */
            AESCipherBlock(u8 const* data, size_t length, PaddingMode mode = PaddingMode::CMS)
                : AESCipherBlock(mode)
            {
                CipherBlock::overwrite(data, length);
            }

            /**
             * @return constexpr size_t 
             */
            constexpr static size_t block_size() 
            { 
                return BlockSizeInBits / 8; 
            };

            /**
             * @return ReadonlyBytes 
             */
            virtual ReadonlyBytes bytes() const override 
            { 
                return ReadonlyBytes { m_data, sizeof(m_data) }; 
            }

            /**
             * @return Bytes 
             */
            virtual Bytes bytes() override 
            { 
                return Bytes { m_data, sizeof(m_data) }; 
            }

            virtual void overwrite(ReadonlyBytes) override;

            /**
             * @param data 
             * @param size 
             */
            virtual void overwrite(u8 const* data, size_t size) override 
            { 
                overwrite({ data, size }); 
            }

            /**
             * @param ivec 
             */
            virtual void apply_initialization_vector(ReadonlyBytes ivec) override
            {
                for (size_t i = 0; i < min(block_size(), ivec.size()); ++i)
                    m_data[i] ^= ivec[i];
            }

        #ifndef KERNEL
            String to_string() const;
        #endif

        private:
            constexpr static size_t data_size() 
            { 
                return sizeof(m_data); 
            }

            u8 m_data[BlockSizeInBits / 8] {};
        };

        struct AESCipherKey : public CipherKey 
        {
            /**
             * @return ReadonlyBytes 
             */
            virtual ReadonlyBytes bytes() const override 
            { 
                return ReadonlyBytes { m_rd_keys, sizeof(m_rd_keys) }; 
            };

            /**
             * @param user_key 
             * @param bits 
             */
            virtual void expand_encrypt_key(ReadonlyBytes user_key, size_t bits) override;

            /**
             * @param user_key 
             * @param bits 
             */
            virtual void expand_decrypt_key(ReadonlyBytes user_key, size_t bits) override;

            /**
             * @param bits 
             * @return true 
             * @return false 
             */
            static bool is_valid_key_size(size_t bits) 
            { 
                return bits == 128 || bits == 192 || bits == 256; 
            };

        #ifndef KERNEL
            String to_string() const;
        #endif

            /**
             * @return u32 const* 
             */
            u32 const* round_keys() const
            {
                return (u32 const*)m_rd_keys;
            }

            /**
             * @brief Construct a new AESCipherKey object
             * 
             * @param user_key 
             * @param key_bits 
             * @param intent 
             */
            AESCipherKey(ReadonlyBytes user_key, size_t key_bits, Intent intent)
                : m_bits(key_bits)
            {
                if (intent == Intent::Encryption)
                    expand_encrypt_key(user_key, key_bits);
                else
                    expand_decrypt_key(user_key, key_bits);
            }

            /**
             * @brief Destroy the AESCipherKey object
             * 
             */
            virtual ~AESCipherKey() override = default;

            /**
             * @return size_t 
             */
            size_t rounds() const 
            { 
                return m_rounds; 
            }

            /**
             * @return size_t 
             */
            size_t length() const 
            { 
                return m_bits / 8; 
            }

        protected:
            /**
             * @return u32* 
             */
            u32* round_keys()
            {
                return (u32*)m_rd_keys;
            }

        private:
            static constexpr size_t MAX_ROUND_COUNT = 14;
            u32 m_rd_keys[(MAX_ROUND_COUNT + 1) * 4] { 0 };
            size_t m_rounds;
            size_t m_bits;
        }; // struct AESCipherKey : public CipherKey

        class AESCipher final : public Cipher<AESCipherKey, AESCipherBlock> 
        {
        public:
            using CBCMode = CBC<AESCipher>;
            using CTRMode = CTR<AESCipher>;
            using GCMMode = GCM<AESCipher>;

            constexpr static size_t BlockSizeInBits = BlockType::BlockSizeInBits;

            /**
             * @brief Construct a new AESCipher object
             * 
             * @param user_key 
             * @param key_bits 
             * @param intent 
             * @param mode 
             */
            AESCipher(ReadonlyBytes user_key, size_t key_bits, Intent intent = Intent::Encryption, PaddingMode mode = PaddingMode::CMS)
                : Cipher<AESCipherKey, AESCipherBlock>(mode)
                , m_key(user_key, key_bits, intent)
            {
            }

            /**
             * @return AESCipherKey const& 
             */
            virtual AESCipherKey const& key() const override 
            { 
                return m_key; 
            };

            /**
             * @return AESCipherKey& 
             */
            virtual AESCipherKey& key() override 
            { 
                return m_key; 
            };

            /**
             * @param in 
             * @param out 
             */
            virtual void encrypt_block(BlockType const& in, BlockType& out) override;

            /**
             * @param in 
             * @param out 
             */
            virtual void decrypt_block(BlockType const& in, BlockType& out) override;

        #ifndef KERNEL
            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                return "AES";
            }
        #endif

        protected:
            AESCipherKey m_key;
        }; // class AESCipher final : public Cipher<AESCipherKey, AESCipherBlock> 

    } // namespace Cipher

} // namespace Crypto
