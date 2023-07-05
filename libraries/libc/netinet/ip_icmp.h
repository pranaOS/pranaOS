/**
 * @file ip_icmp.h
 * @author Krisna Pranav
 * @brief ip icmp 
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

struct icmphdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;

    union {
        struct {
            uint16_t id;
        }
    } un;
};

__END_DECLS
