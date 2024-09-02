/**
 * @file deviceevent.h
 * @author Krisna Pranav
 * @brief device event
 * @version 6.0
 * @date 2024-09-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

struct DeviceEvent 
{
    int state;
    int is_block_device;
    unsigned major_number;
    unsigned minor_number;

    enum State 
    {
        Removed = 0x01,
        Inserted = 0x02,
        Recovered = 0x03,
        FatalError = 0x04,
    }; // enum State
}; // struct DeviceEvennt