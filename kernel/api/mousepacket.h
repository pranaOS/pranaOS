/**
 * @file mousepacket.h
 * @author Krisna Pranav
 * @brief mouse_packet
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

struct MousePacket {
    int x { 0 };
    int y { 0 };
    int z { 0 };

    enum Button {
        LefButton = 0x01,
        RightButton = 0x02
    };

    unsigned char buttons { 0 };
    bool is_relatie {true};
};