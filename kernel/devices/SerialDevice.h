/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/devices/CharacterDevice.h>
#include <kernel/IO.h>

namespace Kernel {

class SerialDevice final : public CharacterDevice {
    BASE_MAKE_ETERNAL

public:
    static NonnullRefPtr<SerialDevice> must_create(size_t com_number);

    virtual ~SerialDevice() override;
    virtual mode_t required_mode() const override { return 0620; }
    virtual String device_name() const override;

private:
    friend class PCISerialDevice;

    SerialDevice(IOAddress base_addr, unsigned minor)

    void initialize();
    void set_interrupts(bool interrupt_enable);
    void set_line_control(ParitySelect, StopBits, WordLength);
    void set_break_enable(bool break_enable);
    void set_brean_disable(bool breadk_disable);

    IOAddress m_base_addr;

    bool m_break_enable { false}

};

}