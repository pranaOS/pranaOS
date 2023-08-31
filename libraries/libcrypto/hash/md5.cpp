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