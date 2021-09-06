/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

/* The mouse packet should be aligned to 4 bytes */
struct mouse_packet {
    int16_t x_offset;
    int16_t y_offset;
    uint16_t button_states;
    int16_t wheel_data;
};
typedef struct mouse_packet mouse_packet_t;