/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/Types.h>
#include <kernel/bus/usb/USBDescriptors.h>
#include <kernel/vm/Region.h>

namespace Kernel::USB {

class Pipe {
public:
    enum class Type : u8 {
        Control = 0,
        Isochronous = 1,
        Bulk = 2,
        Interrupt = 3
    };

    enum class Direction : u8 {
        Out = 0,
        In = 1,
        Bidirectional = 2
    };

    enum class DeviceSpeed : u8 {
        LowSpeed,
        FullSpeed
    };

public:
    static KResultOr<NonnullOwnPtr<Pipe>> try_create_pipe(Type type, Direction direction, u8 endpoint_address, u16 max_packet_size, i8 device_address, u8 poll_interval = 0);

    Type type() const { return m_type; }
    Direction direction() const { return m_direction; }
    DeviceSpeed device_speed() const { return m_speed; }

    i8 device_address() const { return m_device_address; }
    u8 endpoint_address() const { return m_endpoint_address; }
    u16 max_packet_size() const { return m_max_packet_size; }
    u8 poll_interval() const { return m_poll_interval; }
    bool data_toggle() const { return m_data_toggle; }

    void set_max_packet_size(u16 max_size) { m_max_packet_size = max_size; }
    void set_toggle(bool toggle) { m_data_toggle = toggle; }
    void set_device_address(i8 addr) { m_device_address = addr; }

    KResultOr<size_t> control_transfer(u8 request_type, u8 request, u16 value, u16 index, u16 length, void* data);

    Pipe(Type type, Direction direction, u16 max_packet_size);
    Pipe(Type type, Direction direction, USBEndpointDescriptor& endpoint);
    Pipe(Type type, Direction direction, u8 endpoint_address, u16 max_packet_size, u8 poll_interval, i8 device_address);

private:
    friend class Device;

    Type m_type;
    Direction m_direction;
    DeviceSpeed m_speed;

    i8 m_device_address { 0 };    
    u8 m_endpoint_address { 0 };  
    u16 m_max_packet_size { 0 };  
    u8 m_poll_interval { 0 };    
    bool m_data_toggle { false }; 
};
}