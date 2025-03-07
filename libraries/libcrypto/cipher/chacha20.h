/**
 * @file chacha20.h
 * @author Krisna Pranav
 * @brief chacha 20
 * @version 6.0
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>

namespace Crypto::Cipher 
{

    class ChaCha20 
    {
        static constexpr u32 CONSTANT_16_BYTES[] { 0x61707865, 0x3120646E, 0x79622D36, 0x6B206574 };
        static constexpr u32 CONSTANT_32_BYTES[] { 0x61707865, 0x3320646E, 0x79622D32, 0x6B206574 };

    public:
        /**
         * @brief Construct a new ChaCha20 object
         * 
         * @param key 
         * @param nonce 
         * @param initial_counter 
         */
        ChaCha20(ReadonlyBytes key, ReadonlyBytes nonce, u32 initial_counter = 0);

        /**
         * @param input 
         * @param output 
         */
        void encrypt(ReadonlyBytes input, Bytes& output);
        void decrypt(ReadonlyBytes input, Bytes& output);

    private:
        /**
         * @param input 
         * @param output 
         */
        void run_cipher(ReadonlyBytes input, Bytes& output);

        void generate_block();

        /**
         * @param a 
         * @param b 
         * @param c 
         * @param d 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void do_quarter_round(u32& a, u32& b, u32& c, u32& d);

        u32 m_state[16] {};
        u32 m_block[16] {};
    }; // class ChaCha20 

} // namespace Crypto::Cipher 
