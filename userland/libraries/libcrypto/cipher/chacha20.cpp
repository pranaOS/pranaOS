/**
 * @file chacha20.cpp
 * @author Krisna Pranav
 * @brief chacha20
 * @version 6.0
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/endian.h>
#include <libcrypto/cipher/chacha20.h>

namespace Crypto::Cipher 
{

    /**
     * @brief Construct a new ChaCha20::ChaCha20 object
     * 
     * @param key 
     * @param nonce 
     * @param initial_counter 
     */
    ChaCha20::ChaCha20(ReadonlyBytes key, ReadonlyBytes nonce, u32 initial_counter)
    {
        VERIFY(key.size() == 16 || key.size() == 32);
        VERIFY(nonce.size() == 8 || nonce.size() == 12);

        if (key.size() == 32) {
            m_state[0] = CONSTANT_32_BYTES[0];
            m_state[1] = CONSTANT_32_BYTES[1];
            m_state[2] = CONSTANT_32_BYTES[2];
            m_state[3] = CONSTANT_32_BYTES[3];
        } else {
            m_state[0] = CONSTANT_16_BYTES[0];
            m_state[1] = CONSTANT_16_BYTES[1];
            m_state[2] = CONSTANT_16_BYTES[2];
            m_state[3] = CONSTANT_16_BYTES[3];
        }

        for (u32 i = 0; i < 16; i += 4) {
            m_state[(i / 4) + 4] = Mods::convert_between_host_and_little_endian(ByteReader::load32(key.offset(i)));
        }

        u32 key_offset = key.size() == 32 ? 16 : 0;
        for (u32 i = 0; i < 16; i += 4) {
            m_state[(i / 4) + 8] = Mods::convert_between_host_and_little_endian(ByteReader::load32(key.offset(key_offset + i)));
        }

        m_state[12] = initial_counter;

        u32 nonce_offset = nonce.size() == 8 ? 1 : 0;

        for (u32 i = 0; i < 12; i += 4) {
            m_state[(i / 4) + 13 + nonce_offset] = Mods::convert_between_host_and_little_endian(ByteReader::load32(nonce.offset(i)));
        }
    }

    void ChaCha20::generate_block()
    {
        memcpy(m_block, m_state, 16 * sizeof(u32));

        for (u32 i = 0; i < 20; i += 2) {
            do_quarter_round(m_block[0], m_block[4], m_block[8], m_block[12]);
            do_quarter_round(m_block[1], m_block[5], m_block[9], m_block[13]);
            do_quarter_round(m_block[2], m_block[6], m_block[10], m_block[14]);
            do_quarter_round(m_block[3], m_block[7], m_block[11], m_block[15]);

            do_quarter_round(m_block[0], m_block[5], m_block[10], m_block[15]);
            do_quarter_round(m_block[1], m_block[6], m_block[11], m_block[12]);
            do_quarter_round(m_block[2], m_block[7], m_block[8], m_block[13]);
            do_quarter_round(m_block[3], m_block[4], m_block[9], m_block[14]);
        }

        for (u32 i = 0; i < 16; i++) {
            m_block[i] += m_state[i];
        }

        for (u32 i = 0; i < 16; i++) {
            m_block[i] = Mods::convert_between_host_and_little_endian(m_block[i]);
        }
    }

    /**
     * @param x 
     * @param n 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static void rotl(u32& x, u32 n)
    {
        x = (x << n) | (x >> (32 - n));
    }

    /**
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     */
    void ChaCha20::do_quarter_round(u32& a, u32& b, u32& c, u32& d)
    {
        a += b;
        d ^= a;
        rotl(d, 16);

        c += d;
        b ^= c;
        rotl(b, 12);

        a += b;
        d ^= a;
        rotl(d, 8);

        c += d;
        b ^= c;
        rotl(b, 7);
    }

    /**
     * @param input 
     * @param output 
     */
    void ChaCha20::run_cipher(ReadonlyBytes input, Bytes& output)
    {
        size_t offset = 0;
        size_t block_offset = 0;
        while (offset < input.size()) {
            if (block_offset == 0 || block_offset >= 64) {
                generate_block();

                m_state[12]++;
                if (m_state[12] == 0) {
                    m_state[13]++;
                }

                block_offset = 0;
            }

            u32 n = min(input.size() - offset, 64 - block_offset);
            u8* key_block = (u8*)m_block + block_offset;
            for (u32 i = 0; i < n; i++) {
                u8 input_byte = input.offset_pointer(offset)[i];
                u8 key_byte = key_block[i];
                u8 output_byte = input_byte ^ key_byte;

                ByteReader::store(output.offset_pointer(offset + i), output_byte);
            }

            offset += n;
            block_offset += n;
        }
    }

    /**
     * @param input 
     * @param output 
     */
    void ChaCha20::encrypt(ReadonlyBytes input, Bytes& output)
    {
        VERIFY(input.size() <= output.size());
        this->run_cipher(input, output);
    }

    /**
     * @param input 
     * @param output 
     */
    void ChaCha20::decrypt(ReadonlyBytes input, Bytes& output)
    {
        VERIFY(input.size() <= output.size());
        this->run_cipher(input, output);
    }

} // namespace Crypto::Cipher
