/**
 * @file random.h
 * @author Krisna Pranav
 * @brief random
 * @version 6.0
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/assertions.h>
#include <mods/byte_buffer.h>
#include <mods/types.h>
#include <kernel/lock.h>
#include <kernel/stdlib.h>
#include <kernel/arch/i386/cpu.h>
#include <libcrypto/cipher/cipher.h>
#include <libcrypto/hash/sha2.h>
#include <libcrypto/cipher/aes.h>

namespace Kernel 
{

    template<typename CipherT, typename HashT, int KeySize>
    class FortunaPRNG
    {
    public:
        constexpr static size_t pool_count = 32;
        constexpr static size_t reseed_threshold = 16;

        using CipherType = CipherT;
        using BlockType = CipherT::BlockType;
        using HashType = HashT;
        using DigestType = HashT::DigestType;

        FortunaPRNG()
            : m_counter(ByteBuffer::create_zeroed(BlockType::block_size()))
        {}

        void get_random_bytes(u8* buffer, size_t n)
        {
            if (m_p0_len >= reseed_threshold) {
                this->reseed();
            }

            ASSERT(is_seeded());
        }

        template<typename T>
        void add_random_event(const T& event_data, size_t pool)
        {
            pool %= pool_count;

            if (pool == 0) {
                m_p0_len++;
            }

            m_pools[pool].update(reinterpret_cast<const u8*>(&event_data));
        }
    
    private:

        void reseed()
        {
            HashType new_key;
            new_key.update(m_key);

            for (size_t i = 0; i < pool_count; ++i) {
                if (m_reseed_number % (1 << i) == 0) {
                    DigestType digest = m_pools[i].digest();
                    new_key.update(digest.immutable_data(), digest.data_length());
                }
            }

            DigestType digest = new_key.digest();

            m_key = ByteBuffer::copy(digest.immutable_data(),
                digest.data_length());

            m_reseed_number++;

            m_p0_len = 0;
        }

        ByteBuffer m_counter;
        ByteBuffer m_key;
        size_t m_reseed_number { 0 };
        size_t m_p0_len { 0 };

        HashType m_pools[pool_count];
    }; // class FortunaPRNG

} // namespace Kernel