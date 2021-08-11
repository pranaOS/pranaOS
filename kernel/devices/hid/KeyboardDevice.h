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
#include <kernel/devices/CharacterDevice.h>
#include <kernel/devices/hid/HIDDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Random.h>

namespace Kernel {

class KeyboardDevice : public HIDDevice {
public:
    using Event = KeyEvent;

    virtual ~KeyboardDevice() override;

    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual bool can_write(const FileDescription&, size_t) const override { return true; }

    virtual Type instrument_type() const override { return Type::Keyboard; }

    virtual mode_t required_mode() const override { return 0440; }

    virtual KResult ioctl(FileDescription&, unsigned request, Userspace<void*> arg) override;

    virtual String device_name() const override { return String::formatted("keyboard{}", minor()); }

    void update_modifier(u8 modifier, bool state)
    {
        if (state)
            m_modifiers |= modifier;
        else
            m_modifiers &= ~modifier;
    }

protected:
    KeyboardDevice();
    mutable SpinLock<u8> m_queue_lock;
    CircularQueue<Event, 16> m_queue;

    virtual StringView class_name() const override { return "KeyboardDevice"; }

    u8 m_modifiers { 0 };
    bool m_caps_lock_to_ctrl_pressed { false };
    bool m_caps_lock_on { false };
    bool m_num_lock_on { false };
    bool m_has_e0_prefix { false };
    bool m_both_shift_keys_pressed { false };

    void key_state_changed(u8 raw, bool pressed);
};
}
