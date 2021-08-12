/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/Types.h>
#include <kernel/bus/usb/USBPipe.h>

namespace Kernel::USB {

class USBController;

class Device : public RefCounted<Device> {
public:
    enum class PortNumber : u8 {
        Port1 = 0,
        Port2
    };

    enum class DeviceSpeed : u8 {
        FullSpeed = 0,
        LowSpeed
    };

public:
    static KResultOr<NonnullRefPtr<Device>> try_create(USBController const&, PortNumber, DeviceSpeed);

    Device(USBController const&, PortNumber, DeviceSpeed, NonnullOwnPtr<Pipe> default_pipe);
    ~Device();

    KResult enumerate();

    PortNumber port() const { return m_device_port; }
    DeviceSpeed speed() const { return m_device_speed; }

    u8 address() const { return m_address; }

    const USBDeviceDescriptor& device_descriptor() const { return m_device_descriptor; }

private:
    PortNumber m_device_port;   
    DeviceSpeed m_device_speed; 
    u8 m_address { 0 };         

    u16 m_vendor_id { 0 };                   
    u16 m_product_id { 0 };                  
    USBDeviceDescriptor m_device_descriptor; 

    NonnullRefPtr<USBController> m_controller;
    NonnullOwnPtr<Pipe> m_default_pipe; 
};
}
