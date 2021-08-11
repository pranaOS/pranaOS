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
#include <kernel/devices/hid/PS2MouseDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Random.h>

namespace Kernel {

class VMWareMouseDevice final : public PS2MouseDevice {
public:
    static RefPtr<VMWareMouseDevice> try_to_initialize(const I8042Controller&);
    virtual ~VMWareMouseDevice() override;

    virtual void irq_handle_byte_read(u8 byte) override;

private:
    explicit VMWareMouseDevice(const I8042Controller&);
};

}
