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

#include <libcrypto/hash/hashfunction.h>

#ifndef KERNEL
#    include <mods/string.h>
#endif

namespace Crypto 
{
    namespace Hash 
    {

        namespace SHA1Constants 
        {

            constexpr static u32 InitializationHashes[5] { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };

            constexpr static u32 RoundConstants[4] {
                0X5a827999,
                0X6ed9eba1,
                0X8f1bbcdc,
                0Xca62c1d6,
            };

        } // namespace SHA1Constants 

        class SHA1 final : public HashFunction<512, 160> 
        {
        public:
            using HashFunction::update;

            /**
             * @brief Construct a new SHA1 object
             * 
             */
            SHA1()
            {
                reset();
            }

            virtual void update(u8 const*, size_t) override;

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
            inline static DigestType hash(u8 const* data, size_t length)
            {
                SHA1 sha;
                sha.update(data, length);
                return sha.digest();
            }

            /**
             * @param buffer 
             * @return DigestType 
             */
            inline static DigestType hash(ByteBuffer const& buffer) 
            { 
                return hash(buffer.data(), buffer.size()); 
            }

            /**
             * @param buffer 
             * @return DigestType 
             */
            inline static DigestType hash(StringView buffer) 
            { 
                return hash((u8 const*)buffer.characters_without_null_termination(), buffer.length()); 
            }

        #ifndef KERNEL
            virtual String class_name() const override
            {
                return "SHA1";
            }
        #endif

            inline virtual void reset() override
            {
                m_data_length = 0;
                m_bit_length = 0;
                for (auto i = 0; i < 5; ++i)
                    m_state[i] = SHA1Constants::InitializationHashes[i];
            }

        private:
            inline void transform(u8 const*);

            u8 m_data_buffer[BlockSize] {};
            size_t m_data_length { 0 };

            u64 m_bit_length { 0 };
            u32 m_state[5];

            constexpr static auto FinalBlockDataSize = BlockSize - 8;
            constexpr static auto Rounds = 80;
        }; // class SHA1 final : public HashFunction<512, 160>

    } // namespace Hash

} // namespace Crypto
