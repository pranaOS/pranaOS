/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/CircularQueue.h>
#include <kernel/api/MousePacket.h>
#include <kernel/devices/hid/I8042Controller.h>
#include <kernel/devices/hid/MouseDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Random.h>

namespace Kernel {
class PS2MouseDevice : public IRQHandler
    , public MouseDevice
    , public I8042Device {
public:
    static RefPtr<PS2MouseDevice> try_to_initialize(const I8042Controller&);
    bool initialize();

    virtual ~PS2MouseDevice() override;

    virtual StringView purpose() const override { return class_name(); }

    virtual void irq_handle_byte_read(u8 byte) override;
    virtual void enable_interrupts() override
    {
        enable_irq();
    }

protected:
    explicit PS2MouseDevice(const I8042Controller&);

    virtual bool handle_irq(const RegisterState&) override;

    struct RawPacket {
        union {
            u32 dword;
            u8 bytes[4];
        };
    };

    u8 read_from_device();
    u8 send_command(u8 command);
    u8 send_command(u8 command, u8 data);
    MousePacket parse_data_packet(const RawPacket&);
    void set_sample_rate(u8);
    u8 get_device_id();

    u8 m_data_state { 0 };
    RawPacket m_data;
    bool m_has_wheel { false };
    bool m_has_five_buttons { false };
};

}
