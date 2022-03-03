/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <listing/list.h>
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
    struct sock *sk;
    struct net_device *dev;
    struct list_head sibling;

    uint32_t len, true_size;

    union {
        struct udp_packet *udpht;
        struct tcp_packet *tcpht;
        struct icmp_packet *icmpt;

    } t; 

    union {
        struct ip4_packet *ipnt;
        struct arp_packet *arpnt;
        uint8_t *raw;
    } nt;

    union {
        struct ethernet_packet *eh;
        uint8_t *raw;
    } mac;

    uint8_t *head;
    uint8_t *data;
    uint8_t *tail;

    char cb[40];
};