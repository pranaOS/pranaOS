/**
 * @file md5.cpp
 * @author Krisna Pranav
 * @brief md5
 * @version 6.0
 * @date 2023-08-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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

static constexpr void round_1(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
{
    a += F(b, c, d) + x + ac;
    a = ROTATE_LEFT(a, s);
    a += b;
}

/**
 * 
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
        MD5::DigestType MD5::digest()
        {
            auto digest = peek();
            reset();
            return digest;
        }

        MD5::DigestType MD5::peek()
        {
            DigestType digest;

            u8 bits[8];

            return digest;
        }
    }
}