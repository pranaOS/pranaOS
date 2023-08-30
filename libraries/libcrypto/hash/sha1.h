/**
 * @file sha1.h
 * @author Krisna Pranav
 * @brief sha1
 * @version 6.0
 * @date 2023-08-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
            constexpr static u32 InitializationHashes[5] { };
            
            constexpr static u32 RoundConstants[4] {};
        } // namespace SHA1Constatns

        template<size_t Bytes>
        struct SHA1Digest
        {
            u8 data[Bytes];
            constexpr static size_t Size = Bytes;

            /**
             * @return const u8* 
             */
            const u8* immutable_data() const
            {
                return data;
            }

            /**
             * @return size_t 
             */
            size_t data_length() 
            {
                return Bytes;
            }
        }; // struct

        class SHA1 final : public HashFunction<512, SHA1Digest<160 / 8>> 
        {
        public:
            /// @brief Construct a new SHA1 object
            SHA1()
            {
                reset();
            };

            /// @brief: reset
            inline virtual void reset() override
            {
                m_data_length = 0;
                m_bit_length = 0;

                for (auto i = 0; i < 5; ++i)    
                    m_state[i] = SHA1Constants::InitializationHashes[i];
            }

        private:
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