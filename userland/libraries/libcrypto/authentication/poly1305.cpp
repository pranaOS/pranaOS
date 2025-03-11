/**
 * @file poly1305.cpp
 * @author Krisna Pranav
 * @brief poly1305
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/endian.h>
#include <libcrypto/authentication/poly1305.h>

namespace Crypto::Authentication 
{

    /**
     * @brief Construct a new Poly1305::Poly1305 object
     * 
     * @param key 
     */
    Poly1305::Poly1305(ReadonlyBytes key)
    {
        for (size_t i = 0; i < 16; i += 4) {
            m_state.r[i / 4] = Mods::convert_between_host_and_little_endian(ByteReader::load32(key.offset(i)));
        }

        m_state.r[0] &= 0x0FFFFFFF;
        m_state.r[1] &= 0x0FFFFFFC;
        m_state.r[2] &= 0x0FFFFFFC;
        m_state.r[3] &= 0x0FFFFFFC;

        for (size_t i = 16; i < 32; i += 4) {
            m_state.s[(i - 16) / 4] = Mods::convert_between_host_and_little_endian(ByteReader::load32(key.offset(i)));
        }
    }

    /**
     * @param message 
     */
    void Poly1305::update(ReadonlyBytes message)
    {
        size_t offset = 0;
        while (offset < message.size()) {
            u32 n = min(message.size() - offset, 16 - m_state.block_count);
            memcpy(m_state.blocks + m_state.block_count, message.offset_pointer(offset), n);
            m_state.block_count += n;
            offset += n;

            if (m_state.block_count == 16) {
                process_block();
                m_state.block_count = 0;
            }
        }
    }

    void Poly1305::process_block()
    {
        u32 a[5];
        u8 n = m_state.block_count;

        m_state.blocks[n++] = 0x01;

        while (n < 17) {
            m_state.blocks[n++] = 0x00;
        }

        for (size_t i = 0; i < 16; i += 4) {
            a[i / 4] = Mods::convert_between_host_and_little_endian(ByteReader::load32(m_state.blocks + i));
        }

        a[4] = m_state.blocks[16];

        m_state.a[0] += a[0];
        m_state.a[1] += a[1];
        m_state.a[2] += a[2];
        m_state.a[3] += a[3];
        m_state.a[4] += a[4];

        m_state.a[1] += m_state.a[0] >> 32;
        m_state.a[2] += m_state.a[1] >> 32;
        m_state.a[3] += m_state.a[2] >> 32;
        m_state.a[4] += m_state.a[3] >> 32;

        a[0] = m_state.a[0] & 0xFFFFFFFF;
        a[1] = m_state.a[1] & 0xFFFFFFFF;
        a[2] = m_state.a[2] & 0xFFFFFFFF;
        a[3] = m_state.a[3] & 0xFFFFFFFF;
        a[4] = m_state.a[4] & 0xFFFFFFFF;

        m_state.a[0] = (u64)a[0] * m_state.r[0];
        m_state.a[1] = (u64)a[0] * m_state.r[1] + (u64)a[1] * m_state.r[0];
        m_state.a[2] = (u64)a[0] * m_state.r[2] + (u64)a[1] * m_state.r[1] + (u64)a[2] * m_state.r[0];
        m_state.a[3] = (u64)a[0] * m_state.r[3] + (u64)a[1] * m_state.r[2] + (u64)a[2] * m_state.r[1] + (u64)a[3] * m_state.r[0];
        m_state.a[4] = (u64)a[1] * m_state.r[3] + (u64)a[2] * m_state.r[2] + (u64)a[3] * m_state.r[1] + (u64)a[4] * m_state.r[0];
        m_state.a[5] = (u64)a[2] * m_state.r[3] + (u64)a[3] * m_state.r[2] + (u64)a[4] * m_state.r[1];
        m_state.a[6] = (u64)a[3] * m_state.r[3] + (u64)a[4] * m_state.r[2];
        m_state.a[7] = (u64)a[4] * m_state.r[3];

        m_state.a[1] += m_state.a[0] >> 32;
        m_state.a[2] += m_state.a[1] >> 32;
        m_state.a[3] += m_state.a[2] >> 32;
        m_state.a[4] += m_state.a[3] >> 32;
        m_state.a[5] += m_state.a[4] >> 32;
        m_state.a[6] += m_state.a[5] >> 32;
        m_state.a[7] += m_state.a[6] >> 32;

        a[0] = m_state.a[4] & 0xFFFFFFFC;
        a[1] = m_state.a[5] & 0xFFFFFFFF;
        a[2] = m_state.a[6] & 0xFFFFFFFF;
        a[3] = m_state.a[7] & 0xFFFFFFFF;

        m_state.a[0] &= 0xFFFFFFFF;
        m_state.a[1] &= 0xFFFFFFFF;
        m_state.a[2] &= 0xFFFFFFFF;
        m_state.a[3] &= 0xFFFFFFFF;
        m_state.a[4] &= 0x00000003;

        m_state.a[0] += a[0];
        m_state.a[0] += (a[0] >> 2) | (a[1] << 30);
        m_state.a[1] += a[1];
        m_state.a[1] += (a[1] >> 2) | (a[2] << 30);
        m_state.a[2] += a[2];
        m_state.a[2] += (a[2] >> 2) | (a[3] << 30);
        m_state.a[3] += a[3];
        m_state.a[3] += (a[3] >> 2);

        m_state.a[1] += m_state.a[0] >> 32;
        m_state.a[2] += m_state.a[1] >> 32;
        m_state.a[3] += m_state.a[2] >> 32;
        m_state.a[4] += m_state.a[3] >> 32;

        a[0] = m_state.a[4] & 0xFFFFFFFC;

        m_state.a[0] &= 0xFFFFFFFF;
        m_state.a[1] &= 0xFFFFFFFF;
        m_state.a[2] &= 0xFFFFFFFF;
        m_state.a[3] &= 0xFFFFFFFF;
        m_state.a[4] &= 0x00000003;

        m_state.a[0] += a[0];
        m_state.a[0] += a[0] >> 2;

        m_state.a[1] += m_state.a[0] >> 32;
        m_state.a[2] += m_state.a[1] >> 32;
        m_state.a[3] += m_state.a[2] >> 32;
        m_state.a[4] += m_state.a[3] >> 32;

        m_state.a[0] &= 0xFFFFFFFF;
        m_state.a[1] &= 0xFFFFFFFF;
        m_state.a[2] &= 0xFFFFFFFF;
        m_state.a[3] &= 0xFFFFFFFF;
        m_state.a[4] &= 0x00000003;
    }

    /**
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> Poly1305::digest()
    {
        if (m_state.block_count != 0)
            process_block();

        u32 b[4];

        b[0] = m_state.a[0] & 0xFFFFFFFF;
        b[1] = m_state.a[1] & 0xFFFFFFFF;
        b[2] = m_state.a[2] & 0xFFFFFFFF;
        b[3] = m_state.a[3] & 0xFFFFFFFF;

        m_state.a[0] += 5;

        m_state.a[1] += m_state.a[0] >> 32;
        m_state.a[2] += m_state.a[1] >> 32;
        m_state.a[3] += m_state.a[2] >> 32;
        m_state.a[4] += m_state.a[3] >> 32;

        u32 mask = ((m_state.a[4] & 0x04) >> 2) - 1;

        m_state.a[0] = (m_state.a[0] & ~mask) | (b[0] & mask);
        m_state.a[1] = (m_state.a[1] & ~mask) | (b[1] & mask);
        m_state.a[2] = (m_state.a[2] & ~mask) | (b[2] & mask);
        m_state.a[3] = (m_state.a[3] & ~mask) | (b[3] & mask);

        m_state.a[0] += m_state.s[0];
        m_state.a[1] += m_state.s[1];
        m_state.a[2] += m_state.s[2];
        m_state.a[3] += m_state.s[3];

        m_state.a[1] += m_state.a[0] >> 32;
        m_state.a[2] += m_state.a[1] >> 32;
        m_state.a[3] += m_state.a[2] >> 32;
        m_state.a[4] += m_state.a[3] >> 32;

        b[0] = m_state.a[0] & 0xFFFFFFFF;
        b[1] = m_state.a[1] & 0xFFFFFFFF;
        b[2] = m_state.a[2] & 0xFFFFFFFF;
        b[3] = m_state.a[3] & 0xFFFFFFFF;

        ByteBuffer output = TRY(ByteBuffer::create_uninitialized(16));

        for (auto i = 0; i < 4; i++) {
            ByteReader::store(output.offset_pointer(i * 4), Mods::convert_between_host_and_little_endian(b[i]));
        }

        return output;
    } 

} // namspace Crypto::Authentication
