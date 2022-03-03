/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief arp packet
 * 
 */
void arp_packet();

/**
 * @brief Get the packet object
 * 
 * @return void* 
 */
void* get_packet();

/**
 * @brief initialize
 * 
 * @return true 
 * @return false 
 */
bool init();