/**
 * @file ip_icmp.h
 * @author Krisna Pranav
 * @brief ip icmp 
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

// icmphdr
struct icmphdr {
    uint8_t type; // type[uint8]
    uint8_t code; // code[uint8]
    uint16_t checksum; // checksum[uint16]
    union {
        struct {
            uint16_t id; // id
            uint16_t sequence; // sequence
        } echo;
        uint32_t gateway;
    } un;
};

__END_DECLS
