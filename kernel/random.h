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

    /**
     * @tparam CipherT 
     * @tparam HashT 
     * @tparam KeySize 
     */
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

        /// @brief Construct a new FortunaPRNG object
        FortunaPRNG()
            : m_counter(ByteBuffer::create_zeroed(BlockType::block_size()))
        {
        }

        /**
         * @param buffer 
         * @param n 
         */
        void get_random_bytes(u8* buffer, size_t n)
        {
            if (m_p0_len >= reseed_threshold) {
                this->reseed();
            }

            ASSERT(is_seeded());

            ASSERT(n < (1 << 20));

            typename CipherType::CTRMode cipher(m_key, KeySize, Crypto::Cipher::Intent::Encryption);

            Bytes buffer_span { buffer, n };
            auto counter_span = m_counter.bytes();
            cipher.key_stream(buffer_span, counter_span, &counter_span);

            Bytes key_span = m_key.bytes();
            cipher.key_stream(key_span, counter_span, &counter_span);
        }

        /**
         * @tparam T 
         * @param event_data 
         * @param pool 
         */
        template<typename T>
        void add_random_event(const T& event_data, size_t pool)
        {
            pool %= pool_count;
            if (pool == 0) {
                m_p0_len++;
            }
            m_pools[pool].update(reinterpret_cast<const u8*>(&event_data), sizeof(T));
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_seeded() const
        {
            return m_reseed_number > 0;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_ready() const
        {
            return is_seeded() || m_p0_len >= reseed_threshold;
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
        size_t m_reseed_number { 0 };
        size_t m_p0_len { 0 };
        ByteBuffer m_key;
        HashType m_pools[pool_count];
    }; // class FortunaPRNG

    class KernelRng : public Lockable<FortunaPRNG<Crypto::Cipher::AESCipher, Crypto::Hash::SHA256, 256>> 
    {
        MOD_MAKE_ETERNAL;

    public:
        /// @brief Construct a new Kernel Rng object
        KernelRng();

        static KernelRng& the();

        void wait_for_entropy();

        void wake_if_ready();

    private:
        WaitQueue m_seed_queue;
    }; // class KernelRng

    class EntropySource 
    {
        template<typename T>
        struct Event 
        {
            u64 timestamp;
            size_t source;
            T event_data;
        };

    public:
        /// @brief Construct a new Entropy Source object
        EntropySource()
            : m_source(next_source++)
        {
        }

        /**
         * @tparam T 
         * @param event_data 
         */
        template<typename T>
        void add_random_event(const T& event_data)
        {
            Event<T> event = { read_tsc(), m_source, event_data };
            KernelRng::the().resource().add_random_event(event, m_pool);
            m_pool++;
            KernelRng::the().wake_if_ready();
        }

    private:
        static size_t next_source;
        size_t m_pool { 0 };
        size_t m_source;
        Lock m_lock;
    }; // class EntropySource

    void get_fast_random_bytes(u8*, size_t);
    void get_good_random_bytes(u8*, size_t);

    /**
     * @tparam T 
     * @return T 
     */
    template<typename T>
    inline T get_fast_random()
    {
        T value;
        get_fast_random_bytes(reinterpret_cast<u8*>(&value), sizeof(T));
        return value;
    }

    /**
     * @tparam T 
     * @return T 
     */
    template<typename T>
    inline T get_good_random()
    {
        T value;
        get_good_random_bytes(reinterpret_cast<u8*>(&value), sizeof(T));
        return value;
    }
} // namespace Kernel