/**
 * @file cipher.h
 * @author Krisna Pranav
 * @brief cipher
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/span.h>
#include <mods/types.h>

namespace Crypto 
{
    namespace Cipher 
    {

        enum class Intent {
            Encryption,
            Decryption,
        }; // enum class Intent

        enum class PaddingMode {
            CMS,     
            RFC5246, 
            Null,
            Bit,
            Random,
            Space,
            ZeroLength,
        }; // enum class PaddingMode

        /**
         * @tparam B 
         * @tparam T 
         */
        template<typename B, typename T>
        class Cipher;

        struct CipherBlock 
        {
        public:
            /**
             * @brief Construct a new CipherBlock object
             * 
             * @param mode 
             */
            explicit CipherBlock(PaddingMode mode)
                : m_padding_mode(mode)
            {
            }

            /**
             * @return ReadonlyBytes 
             */
            virtual ReadonlyBytes bytes() const = 0;

            virtual void overwrite(ReadonlyBytes) = 0;

            /**
             * @param data 
             * @param size 
             */
            virtual void overwrite(u8 const* data, size_t size) 
            { 
                overwrite({ data, size }); 
            }

            /**
             * @param ivec 
             */
            virtual void apply_initialization_vector(ReadonlyBytes ivec) = 0;

            /**
             * @return PaddingMode 
             */
            PaddingMode padding_mode() const 
            { 
                return m_padding_mode; 
            }

            /**
             * @brief Set the padding mode object
             * 
             * @param mode 
             */
            void set_padding_mode(PaddingMode mode) 
            { 
                m_padding_mode = mode; 
            }

            /**
             * @tparam T 
             * @param offset 
             * @param value 
             */
            template<typename T>
            void put(size_t offset, T value)
            {
                VERIFY(offset + sizeof(T) <= bytes().size());
                auto* ptr = bytes().offset_pointer(offset);
                auto index { 0 };

                VERIFY(sizeof(T) <= 4);

                if constexpr (sizeof(T) > 3)
                    ptr[index++] = (u8)(value >> 24);

                if constexpr (sizeof(T) > 2)
                    ptr[index++] = (u8)(value >> 16);

                if constexpr (sizeof(T) > 1)
                    ptr[index++] = (u8)(value >> 8);

                ptr[index] = (u8)value;
            }

        protected:
            virtual ~CipherBlock() = default;

        private:
            virtual Bytes bytes() = 0;
            PaddingMode m_padding_mode;
        }; // struct CipherBlock

        struct CipherKey {
            /**
             * @return ReadonlyBytes 
             */
            virtual ReadonlyBytes bytes() const = 0;

            /**
             * @return true 
             * @return false 
             */
            static bool is_valid_key_size(size_t) 
            { 
                return false; 
            };

            virtual ~CipherKey() = default;

        protected:
            /**
             * @param user_key 
             * @param bits 
             */
            virtual void expand_encrypt_key(ReadonlyBytes user_key, size_t bits) = 0;

            /**
             * @param user_key 
             * @param bits 
             */
            virtual void expand_decrypt_key(ReadonlyBytes user_key, size_t bits) = 0;
            size_t bits { 0 };
        }; // struct CipherKey

        /**
         * @tparam KeyT 
         * @tparam BlockT 
         */
        template<typename KeyT = CipherKey, typename BlockT = CipherBlock>
        class Cipher 
        {
        public:
            using KeyType = KeyT;
            using BlockType = BlockT;

            /**
             * @brief Construct a new Cipher<KeyT, BlockT> object
             * 
             * @param mode 
             */
            explicit Cipher<KeyT, BlockT>(PaddingMode mode)
                : m_padding_mode(mode)
            {
            }

            /**
             * @return KeyType const& 
             */
            virtual KeyType const& key() const = 0;
            virtual KeyType& key() = 0;

            /**
             * @return constexpr size_t 
             */
            constexpr static size_t block_size() 
            { 
                return BlockType::block_size(); 
            }

            /**
             * @return PaddingMode 
             */
            PaddingMode padding_mode() const 
            { 
                return m_padding_mode; 
            }

            /**
             * @param in 
             * @param out 
             */
            virtual void encrypt_block(BlockType const& in, BlockType& out) = 0;

            /**
             * @param in 
             * @param out 
             */
            virtual void decrypt_block(BlockType const& in, BlockType& out) = 0;

        #ifndef KERNEL
            virtual String class_name() const = 0;
        #endif

        protected:
            virtual ~Cipher() = default;

        private:
            PaddingMode m_padding_mode;
        }; // class Cipher

    } // namespace Cipher

} // namespace Crypto
