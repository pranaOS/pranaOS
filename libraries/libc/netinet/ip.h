/**
 * @file ip.h
 * @author Krisna Pranav
 * @brief ip
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <assert.h>
#include <bits/stdint.h>
#include <sys/cdefs.h>
#include "in.h"

__BEGIN_DECLS

struct ip {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint8_t ip_v : 4;
    uint8_t ip_hl : 4;
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t ip_hl : 4;
    uint8_t ip_v : 4;
#endif
    uint8_t ip_tos;
    uint16_t ip_len;
    uint16_t ip_id;
    uint16_t ip_off;
    uint8_t ip_ttl;
    uint8_t ip_p;
    uint16_t ip_sum;
    struct in_addr ip_src;
    struct in_addr ip_dst;
} __attribute__((packed));
static_assert(sizeof(struct ip) == 20, "struct ip: invalid length");

__END_DECLS
