#pragma once

#include <stdint.h>

struct ipv4_packet {
    uint8_t version_ihl;
    uint8_t dscp_ecn;
    uint16_t flags_fragment;
} __attribute__ ((packed)) __attribute__((aligned(2)));