/**
 * @file md5.cpp
 * @author Krisna Pranav
 * @brief md5
 * @version 6.0
 * @date 2023-08-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <libcrypto/hash/md5.h>

/**
 * @param x 
 * @param y 
 * @param z 
 * @return constexpr u32 
 */
static constexpr u32 F(u32 x, u32 y, u32 z) 
{ 
    return (x & y) | ((~x) & z); 
};

/**
 * @param x 
 * @param y 
 * @param z 
 * @return constexpr u32 
 */
static constexpr u32 G(u32 x, u32 y, u32 z) 
{ 
    return (x & z) | ((~z) & y); 
};

/**
 * @param x 
 * @param y 
 * @param z 
 * @return constexpr u32 
 */
static constexpr u32 H(u32 x, u32 y, u32 z) 
{ 
    return x ^ y ^ z; 
};

/**
 * @param x 
 * @param y 
 * @param z 
 * @return constexpr u32 
 */
static constexpr u32 I(u32 x, u32 y, u32 z) 
{ 
    return y ^ (x | ~z); 
};

/**
 * @param x 
 * @param n 
 * @return constexpr u32 
 */
static constexpr u32 ROTATE_LEFT(u32 x, size_t n)
{
    return (x << n) | (x >> (32 - n));
}

/**
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @param x 
 * @param s 
 * @param ac 
 */
static constexpr void round_1(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
{
    a += F(b, c, d) + x + ac;
    a = ROTATE_LEFT(a, s);
    a += b;
}

/**
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @param x 
 * @param s 
 * @param ac 
 */
static constexpr void round_2(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
{
    a += G(b, c, d) + x + ac;
    a = ROTATE_LEFT(a, s);
    a += b;
}

/**
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @param x 
 * @param s 
 * @param ac 
 */
static constexpr void round_3(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
{
    a += H(b, c, d) + x + ac;
    a = ROTATE_LEFT(a, s);
    a += b;
}

/**
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @param x 
 * @param s 
 * @param ac 
 */
static constexpr void round_4(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
{
    a += I(b, c, d) + x + ac;
    a = ROTATE_LEFT(a, s);
    a += b;
}

namespace Crypto 
{
    namespace Hash 
    {

        /**
         * @param input 
         * @param length 
         */
        void MD5::update(const u8* input, size_t length)
        {
            auto index = (u32)(m_count[0] >> 3) & 0x3f;

            size_t offset { 0 };

            m_count[0] += (u32)length << 3;

            if (m_count[0] < ((u32)length << 3)) {
                ++m_count[1];
            }

            m_count[1] += (u32)length >> 29;

            auto part_length = 64 - index;

            if (length >= part_length) {
                m_buffer.overwrite(index, input, part_length);
                transform(m_buffer.data());

                for (offset = part_length; offset + 63 < length; offset += 64)
                    transform(&input[offset]);

                index = 0;
            }

            ASSERT(length < part_length || length - offset <= 64);
            m_buffer.overwrite(index, &input[offset], length - offset);
        }

        /**
         * @return MD5::DigestType 
         */
        MD5::DigestType MD5::digest()
        {
            auto digest = peek();
            reset();
            return digest;
        }

        /**
         * @return MD5::DigestType 
         */
        MD5::DigestType MD5::peek()
        {
            DigestType digest;
            u8 bits[8];

            encode(m_count, bits, 8);

            u32 index = (u32)((m_count[0] >> 3) & 0x3f);
            u32 pad_length = index < 56 ? 56 - index : 120 - index;

            update(MD5Constants::PADDING, pad_length);
            update(bits, 8);
            encode(&m_A, digest.data, 4 * sizeof(m_A));

            return digest;
        }

        /**
         * @param from 
         * @param to 
         * @param length 
         */
        void MD5::encode(const u32* from, u8* to, size_t length)
        {
            for (size_t i = 0, j = 0; j < length; ++i, j += 4) {
                to[j] = (u8)(from[i] & 0xff);
                to[j + 1] = (u8)((from[i] >> 8) & 0xff);
                to[j + 2] = (u8)((from[i] >> 16) & 0xff);
                to[j + 3] = (u8)((from[i] >> 24) & 0xff);
            }
        }

        /**
         * @param from 
         * @param to 
         * @param length 
         */
        void MD5::decode(const u8* from, u32* to, size_t length)
        {
            for (size_t i = 0, j = 0; j < length; ++i, j += 4)
                to[i] = (((u32)from[j]) | (((u32)from[j + 1]) << 8) | (((u32)from[j + 2]) << 16) | (((u32)from[j + 3]) << 24));
        }

        /**
         * @param block 
         */
        void MD5::transform(const u8* block)
        {
            auto a = m_A;
            auto b = m_B;
            auto c = m_C;
            auto d = m_D;
            u32 x[16];

            decode(block, x, 64);

            round_1(a, b, c, d, x[0], MD5Constants::S11, 0xd76aa478);  
            round_1(d, a, b, c, x[1], MD5Constants::S12, 0xe8c7b756);  
            round_1(c, d, a, b, x[2], MD5Constants::S13, 0x242070db);  
            round_1(b, c, d, a, x[3], MD5Constants::S14, 0xc1bdceee);  
            round_1(a, b, c, d, x[4], MD5Constants::S11, 0xf57c0faf);  
            round_1(d, a, b, c, x[5], MD5Constants::S12, 0x4787c62a);  
            round_1(c, d, a, b, x[6], MD5Constants::S13, 0xa8304613);   
            round_1(b, c, d, a, x[7], MD5Constants::S14, 0xfd469501);  
            round_1(a, b, c, d, x[8], MD5Constants::S11, 0x698098d8);  
            round_1(d, a, b, c, x[9], MD5Constants::S12, 0x8b44f7af);  
            round_1(c, d, a, b, x[10], MD5Constants::S13, 0xffff5bb1);
            round_1(b, c, d, a, x[11], MD5Constants::S14, 0x895cd7be); 
            round_1(a, b, c, d, x[12], MD5Constants::S11, 0x6b901122); 
            round_1(d, a, b, c, x[13], MD5Constants::S12, 0xfd987193); 
            round_1(c, d, a, b, x[14], MD5Constants::S13, 0xa679438e); 
            round_1(b, c, d, a, x[15], MD5Constants::S14, 0x49b40821); 

            round_2(a, b, c, d, x[1], MD5Constants::S21, 0xf61e2562);  
            round_2(d, a, b, c, x[6], MD5Constants::S22, 0xc040b340);  
            round_2(c, d, a, b, x[11], MD5Constants::S23, 0x265e5a51); 
            round_2(b, c, d, a, x[0], MD5Constants::S24, 0xe9b6c7aa);  
            round_2(a, b, c, d, x[5], MD5Constants::S21, 0xd62f105d);   
            round_2(d, a, b, c, x[10], MD5Constants::S22, 0x2441453);   
            round_2(c, d, a, b, x[15], MD5Constants::S23, 0xd8a1e681);  
            round_2(b, c, d, a, x[4], MD5Constants::S24, 0xe7d3fbc8);  
            round_2(a, b, c, d, x[9], MD5Constants::S21, 0x21e1cde6);   
            round_2(d, a, b, c, x[14], MD5Constants::S22, 0xc33707d6);  
            round_2(c, d, a, b, x[3], MD5Constants::S23, 0xf4d50d87);   
            round_2(b, c, d, a, x[8], MD5Constants::S24, 0x455a14ed);   
            round_2(a, b, c, d, x[13], MD5Constants::S21, 0xa9e3e905);  
            round_2(d, a, b, c, x[2], MD5Constants::S22, 0xfcefa3f8);  
            round_2(c, d, a, b, x[7], MD5Constants::S23, 0x676f02d9);   
            round_2(b, c, d, a, x[12], MD5Constants::S24, 0x8d2a4c8a); 

            round_3(a, b, c, d, x[5], MD5Constants::S31, 0xfffa3942);  
            round_3(d, a, b, c, x[8], MD5Constants::S32, 0x8771f681);  
            round_3(c, d, a, b, x[11], MD5Constants::S33, 0x6d9d6122); 
            round_3(b, c, d, a, x[14], MD5Constants::S34, 0xfde5380c); 
            round_3(a, b, c, d, x[1], MD5Constants::S31, 0xa4beea44);  
            round_3(d, a, b, c, x[4], MD5Constants::S32, 0x4bdecfa9);  
            round_3(c, d, a, b, x[7], MD5Constants::S33, 0xf6bb4b60);  
            round_3(b, c, d, a, x[10], MD5Constants::S34, 0xbebfbc70); 
            round_3(a, b, c, d, x[13], MD5Constants::S31, 0x289b7ec6); 
            round_3(d, a, b, c, x[0], MD5Constants::S32, 0xeaa127fa);  
            round_3(c, d, a, b, x[3], MD5Constants::S33, 0xd4ef3085);  
            round_3(b, c, d, a, x[6], MD5Constants::S34, 0x4881d05);   
            round_3(a, b, c, d, x[9], MD5Constants::S31, 0xd9d4d039);  
            round_3(d, a, b, c, x[12], MD5Constants::S32, 0xe6db99e5); 
            round_3(c, d, a, b, x[15], MD5Constants::S33, 0x1fa27cf8); 
            round_3(b, c, d, a, x[2], MD5Constants::S34, 0xc4ac5665);  

            round_4(a, b, c, d, x[0], MD5Constants::S41, 0xf4292244);  
            round_4(d, a, b, c, x[7], MD5Constants::S42, 0x432aff97);   
            round_4(c, d, a, b, x[14], MD5Constants::S43, 0xab9423a7);  
            round_4(b, c, d, a, x[5], MD5Constants::S44, 0xfc93a039);   
            round_4(a, b, c, d, x[12], MD5Constants::S41, 0x655b59c3);  
            round_4(d, a, b, c, x[3], MD5Constants::S42, 0x8f0ccc92);   
            round_4(c, d, a, b, x[10], MD5Constants::S43, 0xffeff47d);  
            round_4(b, c, d, a, x[1], MD5Constants::S44, 0x85845dd1);   
            round_4(a, b, c, d, x[8], MD5Constants::S41, 0x6fa87e4f);   
            round_4(d, a, b, c, x[15], MD5Constants::S42, 0xfe2ce6e0);  
            round_4(c, d, a, b, x[6], MD5Constants::S43, 0xa3014314);   
            round_4(b, c, d, a, x[13], MD5Constants::S44, 0x4e0811a1);  
            round_4(a, b, c, d, x[4], MD5Constants::S41, 0xf7537e82);   
            round_4(d, a, b, c, x[11], MD5Constants::S42, 0xbd3af235);  
            round_4(c, d, a, b, x[2], MD5Constants::S43, 0x2ad7d2bb);   
            round_4(b, c, d, a, x[9], MD5Constants::S44, 0xeb86d391);   

            m_A += a;
            m_B += b;
            m_C += c;
            m_D += d;

            __builtin_memset(x, 0, sizeof(x));
        }
    } // namespace Hash
} // namespace Crypto
