/**
 * @file inet.h
 * @author Krisna Pranav
 * @brief inet
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <inttypes.h>
#include <netinet/in.h>
#include <sys/cdefs.h>
#include <sys/socket.h>

__BEGIN_DECLS

#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46

/**
 * @param af 
 * @param src 
 * @param dst 
 * @return char const* 
 */
char const* inet_ntop(int af, void const* src, char* dst, socklen_t);

/**
 * @param af 
 * @param src 
 * @param dst 
 * @return int 
 */
int inet_pton(int af, char const* src, void* dst);

/**
 * @param cp 
 * @param inp 
 * @return int 
 */
static inline int inet_aton(char const* cp, struct in_addr* inp)
{
    return inet_pton(AF_INET, cp, inp);
}

/**
 * @return char* 
 */
char* inet_ntoa(struct in_addr);

__END_DECLS
