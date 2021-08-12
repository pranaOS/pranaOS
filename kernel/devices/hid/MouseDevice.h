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
#include <kernel/api/MousePacket.h>
#include <kernel/devices/CharacterDevice.h>
#include <kernel/devices/hid/HIDDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Random.h>

namespace Kernel {

class MouseDevice : public HIDDevice {
public:
    virtual ~MouseDevice() override;

    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual bool can_write(const FileDescription&, size_t) const override { return true; }

    virtual Type instrument_type() const override { return Type::Mouse; }

    virtual mode_t required_mode() const override { return 0440; }

    virtual String device_name() const override { return String::formatted("mouse{}", minor()); }

protected:
    MouseDevice();

    virtual StringView class_name() const override { return "MouseDevice"; }

    mutable SpinLock<u8> m_queue_lock;
    CircularQueue<MousePacket, 100> m_queue;
};

}
