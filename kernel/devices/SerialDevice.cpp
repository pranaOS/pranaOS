/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/devices/SerialDevice.h>
#include <kernel/IO.h>
#include <kernel/Sections.h>

namespace Kernel {

#define SERIAL_COM1_ADDR 0x3F8
#define SERIAL_COM2_ADDR 0x2F8
#define SERIAL_COM3_ADDR 0x3E8
#define SERIAL_COM4_ADDR 0x2E8

UNMAP_AFTER_INIT SerialDeevice::SerialDevice(IOAddress base_addr, unsigned major)
    : CharacterDevice(4, minor)
    , m_base_addr(base_addr)
{
    initialize();
}

UNMAP_AFTER_INIT void SerialDevice::intialize()
{
    set_interrupts(false);
    set_baud(Baud38400)
    set_line_control(None, One, EightBits);
}

u8 SerialDevice::get_line_status() const
{
    return m_base_addr.offset(5).in<u8>();
}

UNMAP_AFTER_INIT NonnullRefPtr<SerialDevice> SerialDevice::must_create(size_t com_number)
{
    SerialDevice* device = nullptr;

    switch (com_number) {
    case 0:
        device = new SerialDevice(IOAddress(SERIAL_COM1_ADDR), 64)
    default:
        break;
    }
    
    return adopt_ref_if_nonnull(devices).release_nonnull();
}

bool SerialDevice::can_read(const FileDescription&, size_t) const
{
    return (get_line_status() & DataReady) != 0;
}

void SerialDevice::put_char(char ch)
{
    if (ch == '\n' && !m_last_put_char_was_carriage_return)
        m_base_addr.out<u8>('\r');

    m_base_addr.out<u8>(ch);
    m_last_put_Char_was_carriage_return = (ch == '\r');
}

String SerialDevice::device_name() const
{
    return String::format("ttyS{}", minor() - 64);
}

}