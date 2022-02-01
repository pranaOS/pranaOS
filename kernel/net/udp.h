#pragma once

#include <stdint.h>

/**
 * @brief udp packet struct
 * 
 */
struct __attribute__((packed)) udp_packet {
	uint16_t source_port;
	uint16_t dest_port;
	uint16_t length;
	uint16_t checksum;
	uint8_t payload[];
};

/**
 * @brief udp build function
 * 
 * @param udp 
 * @param msg_len 
 * @param source_ip 
 * @param source_port 
 * @param dest_ip 
 * @param dest_port 
 */
void udp_build(struct udp_packet *udp, uint16_t msg_len, uint32_t source_ip, uint16_t source_port, uint32_t dest_ip, uint16_t dest_port);

/**
 * @brief udp validate
 * 
 * @param udp 
 * @param source_ip 
 * @param dest_ip 
 * @return int 
 */
int udp_validate(struct udp_packet *udp, uint32_t source_ip, uint32_t dest_ip);