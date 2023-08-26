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
            Decryption,
        }; // enum

        enum class PaddingMode 
        {
            CMS,    
            RFC5246,
            Null,
            Bit,
            Random,
            Space,
            ZeroLength,
        }; // enum

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
             * @param mode 
             */
            explicit CipherBlock(PaddingMode mode)
                : m_padding_mode(mode)
            {
            }

            /**
             * @return size_t 
             */
            static size_t block_size() 
            { 
                ASSERT_NOT_REACHED(); 
            }

            /**
             * @return ByteBuffer 
             */
            virtual ByteBuffer get() const = 0;
            virtual const ByteBuffer& data() const = 0;

            virtual void overwrite(ReadonlyBytes) = 0;

            /**
             * @param buffer 
             */
            virtual void overwrite(const ByteBuffer& buffer) 
            { 
                overwrite(buffer.bytes()); 
            }

            /**
             * @param data 
             * @param size 
             */
            virtual void overwrite(const u8* data, size_t size) 
            { 
                overwrite({ data, size }); 
            }

            /**
             * @param ivec 
             */
            virtual void apply_initialization_vector(const u8* ivec) = 0;

            /**
             * @return PaddingMode 
             */
            PaddingMode padding_mode() const 
            { 
                return m_padding_mode; 
            }

            /**
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
                ASSERT(offset + sizeof(T) <= data().size());

                auto* ptr = data().data() + offset;
                auto index { 0 };

                ASSERT(sizeof(T) <= 4);

                if constexpr (sizeof(T) > 3)
                    ptr[index++] = (u8)(value >> 24);

                if constexpr (sizeof(T) > 2)
                    ptr[index++] = (u8)(value >> 16);

                if constexpr (sizeof(T) > 1)
                    ptr[index++] = (u8)(value >> 8);

                ptr[index] = (u8)value;
            }

        private:
            virtual ByteBuffer& data() = 0;
            PaddingMode m_padding_mode;
        };

        struct CipherKey 
        {
            /**
             * @return ByteBuffer 
             */
            virtual ByteBuffer data() const = 0;

            /**
             * @return true 
             * @return false 
             */
            static bool is_valid_key_size(size_t) 
            { 
                return false; 
            };

            virtual ~CipherKey() { }

        protected:
            /**
             * @param user_key 
             * @param bits 
             */
            virtual void expand_encrypt_key(const ByteBuffer& user_key, size_t bits) = 0;
            virtual void expand_decrypt_key(const ByteBuffer& user_key, size_t bits) = 0;

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
             * @param mode 
             */
            explicit Cipher<KeyT, BlockT>(PaddingMode mode)
                : m_padding_mode(mode)
            {
            }

            /**
             * @return const KeyType& 
             */
            virtual const KeyType& key() const = 0;
            virtual KeyType& key() = 0;

            /**
             * @return size_t 
             */
            static size_t block_size() 
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
            virtual void encrypt_block(const BlockType& in, BlockType& out) = 0;
            virtual void decrypt_block(const BlockType& in, BlockType& out) = 0;

            /**
             * @return String 
             */
            virtual String class_name() const = 0;

        private:
            PaddingMode m_padding_mode;
        }; // class Cipher
    } // namespace Cipher
} // namespace Crypto
