#include <errno/error.h>
#include <memory/vmm.h>
#include <net/ethernet.h>
#include <net/ip.h>
#include <net/net.h>
#include <net/buffer.h>
#include <proc/task.h>
#include <ak/math.h>
#include <ak/string.h>
#include "udp.h"

#define MAX_UDP_HEADER (sizeof(struct ethernet_packet) + sizeof(struct ip4_packet) + sizeof(struct udp_packet))

/**
 * @brief udp calculate checksum
 * 
 * @param udp 
 * @param packet_len 
 * @param source_ip 
 * @param dest_ip 
 * @return uint16_t 
 */
uint16_t udp_calculate_checksum(struct udp_packet *udp, uint16_t packet_len, uint32_t source_ip, uint32_t dest_ip) {
    udp->checksum = 0;
    return transport_calculate_checksum(udp, packet_len, IP4_PROTOCAL_UDP, source_ip, dest_ip);
}