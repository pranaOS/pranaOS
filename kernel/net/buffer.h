/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <utilities/list.h>
#include <stdint.h>

/**
 * @brief udp packet
 * 
 */
struct udp_packet;

/**
 * @brief tcp_packet
 * 
 */
struct tcp_packet;

/**
 * @brief icmp packet
 * 
 */
struct icmp_packet;

/**
 * @brief ip4 packet
 * 
 */
struct ip4_packet;

/**
 * @brief arp packet
 * 
 */
struct arp_packet;

/**
 * @brief ethernet packet
 * 
 */
struct ethernet_packet;

/**
 * @brief sk buffer
 * 
 */
struct sk_buff {
    /**
     * @brief socket
     * 
     */
    struct sock *sk;

    /**
     * @brief net device
     * 
     */
    struct net_device *dev;

    /**
     * @brief list head
     * 
     */
    struct list_head sibling;

    /**
     * @brief length, true_size
     * 
     */
    uint32_t len;
    uint32_t true_size;

    /**
     * @brief udp packet, tcp packet, icmp packet
     * 
     */
    union {
        struct udp_packet *udpht;
        struct tcp_packet *tcpht;
        struct icmp_packet *icmpt;

    } t; 

    /**
     * @brief nt[ipv4 packet, arp packet]
     * 
     */
    union {
        struct ip4_packet *ipnt;
        struct arp_packet *arpnt;
        uint8_t *raw;
    } nt;

    /**
     * @brief mac addr 
     * 
     */
    union {
        struct ethernet_packet *eh;
        uint8_t *raw;
    } mac;

    /**
     * @brief head, data, tail, end
     * 
     */
    uint8_t *head;
    uint8_t *data;
    uint8_t *tail;
    uint8_t* end;

    char cb[40];
};

/**
 * @brief skb reserve
 * 
 * @param skb 
 * @param len 
 */
static inline void skb_reserve(struct sk_buff *skb, uint32_t len) {
    skb->data += len;
    skb->tail += len;
}

/**
 * @brief skb put
 * 
 * @param skb 
 * @param len 
 */
static inline void skb_put(struct sk_buff *skb, uint32_t len) {
    skb->tail += len;
    skb->len += len;
}

/**
 * @brief skb push
 * 
 * @param skb 
 * @param len 
 */
static inline void skb_push(struct sk_buff *skb, uint32_t len) {
	skb->data -= len;
	skb->len += len;
}

/**
 * @brief skb pull
 * 
 * @param skb 
 * @param len 
 */
static inline void skb_pull(struct sk_buff *skb, uint32_t len) {
	skb->data += len;
	skb->len -= len;
}

/**
 * @brief skb allocation
 * 
 * @param header_size 
 * @param payload_size 
 * @return struct sk_buff* 
 */
struct sk_buff *skb_alloc(uint32_t header_size, uint32_t payload_size);

/**
 * @brief skb clone 
 * 
 * @param skb 
 * @return struct sk_buff* 
 */
struct sk_buff *skb_clone(struct sk_buff *skb);

/**
 * @brief skb free
 * 
 * @param skb 
 */
void skb_free(struct sk_buff *skb);