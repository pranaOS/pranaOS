/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/CircularQueue.h>
#include <base/DoublyLinkedList.h>
#include <base/Types.h>
#include <kernel/api/KeyCode.h>
#include <kernel/devices/hid/I8042Controller.h>
#include <kernel/devices/hid/KeyboardDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Random.h>
#include <libkeyboard/CharacterMap.h>

namespace Kernel {

class PS2KeyboardDevice final : public IRQHandler
    , public KeyboardDevice
    , public I8042Device {
public:
    static RefPtr<PS2KeyboardDevice> try_to_initialize(const I8042Controller&);
    virtual ~PS2KeyboardDevice() override;
    bool initialize();

    virtual StringView purpose() const override { return class_name(); }

    virtual void irq_handle_byte_read(u8 byte) override;
    virtual void enable_interrupts() override
    {
        enable_irq();
    }

private:
    explicit PS2KeyboardDevice(const I8042Controller&);

    virtual bool handle_irq(const RegisterState&) override;

    virtual StringView class_name() const override { return "KeyboardDevice"; }
};

}
