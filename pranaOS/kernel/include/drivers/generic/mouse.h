/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_DRIVERS_GENERIC_MOUSE_H
#define _KERNEL_DRIVERS_GENERIC_MOUSE_H

struct mouse_packet {
    int16_t x_offset;
    int16_t y_offset;
    uint32_t button_states;
};
typedef struct mouse_packet mouse_packet_t;

#endif 