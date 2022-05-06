#pragma once

#include <stdint.h>

#define IP4_PROTOCAL_ICMP 1
#define IP4_PROTOCAL_TCP 6
#define IP4_PROTOCAL_UDP 17


namespace Kernel {
    struct socket;
    struct sk_buff;

    struct __attribute__((packed)) ip4_pseudo {
        // TODO
    };

    struct __attribute__((packed)) ip4_packet {
        // todo
    };

}