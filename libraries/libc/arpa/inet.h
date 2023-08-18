/**
 * @file inet.h
 * @author Krisna Pranav
 * @brief inet
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <endian.h>
#include <sys/cdefs.h>
#include <sys/socket.h>

__BEGIN_DECLS

#define INET_ADDRSTRLEN 16

/**
 * @param af 
 * @param src 
 * @param dst 
 * @return const char* 
 */
const char* inet_ntop(int af, const void* src, char* dst, socklen_t);

/**
 * @param af 
 * @param src 
 * @param dst 
 * @return int 
 */
int inet_pton(int af, const char* src, void* dst);

/**
 * @param cp 
 * @param inp 
 * @return int 
 */
static inline int inet_aton(const char* cp, struct in_addr* inp) {
    return inet_pton(AF_INET, cp, inp);
}

/**
 * @return char* 
 */
char* inet_ntoa(struct in_addr);

/**
 * @param value 
 * @return uint16_t 
 */
inline uint16_t htons(uint16_t value) {
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
inline uint16_t ntohs(uint16_t value) {
    return htons(value);
}

/**
 * @param value 
 * @return uint32_t 
 */
inline uint32_t htonl(uint32_t value) {
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
inline uint32_t ntohl(uint32_t value) {
    return htonl(value);
}

__END_DECLS
