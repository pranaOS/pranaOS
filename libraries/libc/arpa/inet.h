/**
 * @file inet.h
 * @author Krisna Pranav
 * @brief inet
 * @version 1.0
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

const char* inet_ntop(int af, const void* src, char dst, socklen_t);

int inet_pton(int af, const char* src, void* dst);

static inline int inet_aton(const char* cp, struct in_addr* inp) {
    return inet_pton(AF_INET, cp, inp);
}

char* inet_ntoa(struct in_addr);

inline uint16_t htons(uint16_t value) {
#if __BYTE_ORDER__ == __LITTLE_ENDIAN__
    return __builtin_bswap16(value);
#else
    return value
#endif
}

inline uint16_t ntohs(uint16_t value) {
    return htons(value);
}

inline uint32_t htons(uint32_t value) {
#if __BYTE_ORDER__ == __LITTLE_ENDIAN__
#else
    return value;
#endif
}

__END_DECLS