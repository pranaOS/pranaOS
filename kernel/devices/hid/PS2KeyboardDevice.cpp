/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/ByteBuffer.h>
#include <base/Singleton.h>
#include <base/Types.h>
#include <kernel/Debug.h>
#include <kernel/devices/hid/HIDManagement.h>
#include <kernel/devices/hid/PS2KeyboardDevice.h>
#include <kernel/IO.h>
#include <kernel/Sections.h>
#include <kernel/tty/ConsoleManagement.h>
#include <kernel/WorkQueue.h>

namespace Kernel {

#define IRQ_KEYBOARD 1

void PS2KeyboardDevice::irq_handle_byte_read(u8 byte)
{
    u8 ch = byte & 0x7f;
    bool pressed = !(byte & 0x80);

    m_entropy_source.add_random_event(byte);

    if (byte == 0xe0) {
        m_has_e0_prefix = true;
        return;
    }

    if ((m_modifiers == (Mod_Alt | Mod_Shift) || m_modifiers == (Mod_Ctrl | Mod_Alt | Mod_Shift)) && byte == 0x58) {
        ConsoleManagement::the().switch_to_debug();
        Scheduler::dump_scheduler_state(m_modifiers == (Mod_Ctrl | Mod_Alt | Mod_Shift));
    }

    dbgln_if(KEYBOARD_DEBUG, "Keyboard::irq_handle_byte_read: {:#02x} {}", ch, (pressed ? "down" : "up"));
    switch (ch) {
    case 0x38:
        if (m_has_e0_prefix)
            update_modifier(Mod_AltGr, pressed);
        else
            update_modifier(Mod_Alt, pressed);
        break;
    case 0x1d:
        update_modifier(Mod_Ctrl, pressed);
        break;
    case 0x5b:
        update_modifier(Mod_Super, pressed);
        break;
    case 0x2a:
    case 0x36:
        if (m_both_shift_keys_pressed)
            m_both_shift_keys_pressed = false;
        else if ((m_modifiers & Mod_Shift) != 0 && pressed)
            m_both_shift_keys_pressed = true;
        else
            update_modifier(Mod_Shift, pressed);
        break;
    }
    switch (ch) {
    case I8042_ACK:
        break;
    default:
        if ((m_modifiers & Mod_Alt) != 0 && ch >= 2 && ch <= ConsoleManagement::s_max_virtual_consoles + 1) {
            g_io_work->queue([ch]() {
                ConsoleManagement::the().switch_to(ch - 0x02);
            });
        }
        key_state_changed(ch, pressed);
    }
}

bool PS2KeyboardDevice::handle_irq(const RegisterState&)
{
    return m_i8042_controller->irq_process_input_buffer(HIDDevice::Type::Keyboard);
}

UNMAP_AFTER_INIT RefPtr<PS2KeyboardDevice> PS2KeyboardDevice::try_to_initialize(const I8042Controller& ps2_controller)
{
    auto device = adopt_ref(*new PS2KeyboardDevice(ps2_controller));
    if (device->initialize())
        return device;
    return nullptr;
}

UNMAP_AFTER_INIT bool PS2KeyboardDevice::initialize()
{
    if (!m_i8042_controller->reset_device(HIDDevice::Type::Keyboard)) {
        dbgln("KeyboardDevice: I8042 controller failed to reset device");
        return false;
    }
    return true;
}

UNMAP_AFTER_INIT PS2KeyboardDevice::PS2KeyboardDevice(const I8042Controller& ps2_controller)
    : IRQHandler(IRQ_KEYBOARD)
    , KeyboardDevice()
    , I8042Device(ps2_controller)
{
}

UNMAP_AFTER_INIT PS2KeyboardDevice::~PS2KeyboardDevice()
{
}

}