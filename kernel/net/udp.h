#pragma once

#include <stdint.h>

struct __attribute__((packed)) udp_packet {
    uint16_t source_port;
    uint16_t dest_port;
    uint16_t length;
    uint16_t checksum
    uint16_t payload[];
}

int udp_validate(struct udp_packet *udp, uint32_t source_ip, uint32_t dest_ip);
void udp_build(struct udp_packet* udp, uint16_t msg_len, uint32_t source_ip);