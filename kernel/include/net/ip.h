#ifndef NET_IP_H
#define NET_IP_H

#include <stdint.h>

#define IP4_PROTOCOL_ICMP 1
#define IP4_PROTOCOL_TCP 6
#define IP4_PROTOCOL_UDP 17 

struct socket;
struct sk_buff;

struct ip4_packet *ip4_build_header(struct ip4_packet *packet, uint16_t packet_size, uint8_t protocal, uint32_t source_ip, uint32_t dest_ip, uint32_t identification);
void ip4_sendmsg(struct socket *sock, struct sk_buff *skb);
int ip4_rcv(struct sk_buff *skb);
int ip4_validate_header(struct ip4_packet *ip, uint8_t protocal);

#endif