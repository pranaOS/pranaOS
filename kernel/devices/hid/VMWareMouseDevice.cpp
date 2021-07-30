/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Devices/HID/VMWareMouseDevice.h>
#include <kernel/Devices/VMWareBackdoor.h>
#include <kernel/Sections.h>

namespace Kernel {

UNMAP_AFTER_INIT RefPtr<VMWareMouseDevice> VMWareMouseDevice::try_to_initialize(const I8042Controller& ps2_controller)
{
    if (!VMWareBackdoor::the())
        return {};
    if (!VMWareBackdoor::the()->vmmouse_is_absolute())
        return {};
    auto device = adopt_ref(*new VMWareMouseDevice(ps2_controller));
    if (device->initialize())
        return device;
    return {};
}

void VMWareMouseDevice::irq_handle_byte_read(u8)
{
    VERIFY(VMWareBackdoor::the());
    VERIFY(VMWareBackdoor::the()->vmmouse_is_absolute());
    auto mouse_packet = VMWareBackdoor::the()->receive_mouse_packet();
    if (mouse_packet.has_value()) {
        m_entropy_source.add_random_event(mouse_packet.value());
        {
            ScopedSpinLock lock(m_queue_lock);
            m_queue.enqueue(mouse_packet.value());
        }
        evaluate_block_conditions();
    }
    return;
}

VMWareMouseDevice::VMWareMouseDevice(const I8042Controller& ps2_controller)
    : PS2MouseDevice(ps2_controller)
{
}
VMWareMouseDevice::~VMWareMouseDevice()
{
}

}