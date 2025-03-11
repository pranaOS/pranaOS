/**
 * @file in.h
 * @author Krisna Pranav
 * @brief in
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/netinet/in.h>
#include <endian.h>

__BEGIN_DECLS

/**
 * @return in_addr_t 
 */
in_addr_t inet_addr(char const*);

/**
 * @param value 
 * @return uint16_t 
 */
static inline uint16_t htons(uint16_t value)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap16(value);
#else
    return value;
#endif
}

/**
 * @param value 
 * @return uint16_t 
 */
static inline uint16_t ntohs(uint16_t value)
{
    return htons(value);
}

/**
 * @param value 
 * @return uint32_t 
 */
static inline uint32_t htonl(uint32_t value)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap32(value);
#else
    return value;
#endif
}

/**
 * @param value 
 * @return uint32_t 
 */
static inline uint32_t ntohl(uint32_t value)
{
    return htonl(value);
}

#define IN_MULTICAST(x) (((x)&0xf0000000) == 0xe0000000)

#define IN6_IS_ADDR_LOOPBACK(addr) \
    ((addr)->s6_addr[0] == 0 && (addr)->s6_addr[1] == 0 && (addr)->s6_addr[2] == 0 && (addr)->s6_addr[3] == 0 && (addr)->s6_addr[4] == 0 && (addr)->s6_addr[5] == 0 && (addr)->s6_addr[6] == 0 && (addr)->s6_addr[7] == 0 && (addr)->s6_addr[8] == 0 && (addr)->s6_addr[9] == 0 && (addr)->s6_addr[10] == 0 && (addr)->s6_addr[11] == 0 && (addr)->s6_addr[12] == 0 && (addr)->s6_addr[13] == 0 && (addr)->s6_addr[14] == 0 && (addr)->s6_addr[15] == 1)


#define IN6_IS_ADDR_V4MAPPED(addr) \
    ((((addr)->s6_addr[0]) == 0) && (((addr)->s6_addr[1]) == 0) && (((addr)->s6_addr[2]) == 0) && (((addr)->s6_addr[3]) == 0) && (((addr)->s6_addr[4]) == 0) && (((addr)->s6_addr[5]) == 0) && (((addr)->s6_addr[6]) == 0) && (((addr)->s6_addr[7]) == 0) && (((addr)->s6_addr[8]) == 0) && (((addr)->s6_addr[9]) == 0) && (((addr)->s6_addr[10]) == 0xFF) && (((addr)->s6_addr[11]) == 0xFF))

#define IN6_IS_ADDR_LINKLOCAL(addr) \
    (((addr)->s6_addr[0] == 0xfe) && (((addr)->s6_addr[1] & 0xc0) == 0x80))

__END_DECLS
