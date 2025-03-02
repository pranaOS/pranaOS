/**
 * @file sha1.h
 * @author Krisna Pranav
 * @brief sha1
 * @version 6.0
 * @date 2023-08-30
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <libcrypto/hash/hashfunction.h>

namespace Crypto 
{
    namespace Hash 
    {

        namespace SHA1Constants 
        {

            constexpr static u32 InitializationHashes[5] { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };

            constexpr static u32 RoundConstants[4] 
            {
                0X5a827999,
                0X6ed9eba1,
                0X8f1bbcdc,
                0Xca62c1d6,
            };

        } // namespace SHA1Constants

        /**
         * @tparam Bytes 
         */
        template<size_t Bytes>
        struct SHA1Digest 
        {
            u8 data[Bytes];
            
            constexpr static size_t Size = Bytes;

            /**
             * @return const u8* 
             */
            const u8* immutable_data() const { return data; }

            /**
             * @return size_t 
             */
            size_t data_length() { return Bytes; }
        }; // struct

        class SHA1 final : public HashFunction<512, SHA1Digest<160 / 8>> 
        {
        public:
            /// @brief Construct a new SHA1 object
            SHA1()
            {
                reset();
            }

            /// @brief update
            virtual void update(const u8*, size_t) override;

            /**
             * @param buffer 
             */
            virtual void update(const ByteBuffer& buffer) override 
            { 
                update(buffer.data(), buffer.size()); 
            };

            /**
             * @param string 
             */
            virtual void update(const StringView& string) override 
            { 
                update((const u8*)string.characters_without_null_termination(), string.length()); 
            };

            /**
             * @return DigestType 
             */
            virtual DigestType digest() override;
            virtual DigestType peek() override;

            /**
             * @param data 
             * @param length 
             * @return DigestType 
             */
            inline static DigestType hash(const u8* data, size_t length)
            {
                SHA1 sha;
                sha.update(data, length);
                return sha.digest();
            }

            /**
             * @param buffer 
             * @return DigestType 
             */
            inline static DigestType hash(const ByteBuffer& buffer) 
            { 
                return hash(buffer.data(), buffer.size()); 
            }

            /**
             * @param buffer 
             * @return DigestType 
             */
            inline static DigestType hash(const StringView& buffer) 
            { 
                return hash((const u8*)buffer.characters_without_null_termination(), buffer.length()); 
            }

            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                return "SHA1";
            };

            /// @brief reset
            inline virtual void reset() override
            {
                m_data_length = 0;
                m_bit_length = 0;

                for (auto i = 0; i < 5; ++i)
                    m_state[i] = SHA1Constants::InitializationHashes[i];
            }

        private:
            /// @brief transform
            inline void transform(const u8*);

            u8 m_data_buffer[BlockSize];

            size_t m_data_length { 0 };

            u64 m_bit_length { 0 };

            u32 m_state[5];

            constexpr static auto FinalBlockDataSize = BlockSize - 8;

            constexpr static auto Rounds = 80;
        }; // class SHA1
    } // namespace Hash
} // namespace Crypto