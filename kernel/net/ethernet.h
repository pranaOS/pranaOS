#ifndef KERNEL_ETHERNET_H
#define KERNEL_ETHERNET_H

#include <stdint.h>

struct sk_buff;

struct __attribute__((packed)) ethernet_packet {
	uint8_t dest_mac[6];
	uint8_t source_mac[6];
	uint16_t type;
	uint8_t payload[];
};

/**
 * @brief ethernet build header
 * 
 * @param packet 
 * @param protocol 
 * @param source_mac 
 * @param dest_mac 
 */
void ethernet_build_header(struct ethernet_packet *packet, uint16_t protocol, uint8_t *source_mac, uint8_t *dest_mac);

/**
 * @brief ethernet sendmsg
 * 
 * @param skb 
 */
void ethernet_sendmsg(struct sk_buff *skb);

/**
 * @brief ethernet recive
 * 
 * @param skb 
 * @return int 
 */
int ethernet_rcv(struct sk_buff *skb);

#endif