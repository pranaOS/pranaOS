/*
 * Copyright (c) 2021, AakeshDarsh
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <kernel/devices/CharacterDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/PhysicalPage.h>
#include <kernel/WaitQueue.h>

namespace Kernel {

class SB16;

class SB16 final : public IRQHandler
    , public CharacterDevice {
public:
    SB16();
    virtual ~SB16() override;

    static void detect();
    static void create();
    static SB16& the();

    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual bool can_write(const FileDescription&, size_t) const override { return true; }

    virtual StringView purpose() const override { return class_name(); }

    virtual mode_t required_mode() const override { return 0220; }
    virtual String device_name() const override { return "audio"; }

private:

    virtual bool handle_irq(const RegisterState&) override;

    virtual StringView class_name() const override { return "SB16"; }

    void initialize();
    void wait_for_irq();
    void dma_start(uint32_t length);
    void set_sample_rate(uint16_t hz);
    void dsp_write(u8 value);
    static u8 dsp_read();
    u8 get_irq_line();
    void set_irq_register(u8 irq_number);
    void set_irq_line(u8 irq_number);

    OwnPtr<Region> m_dma_region;
    int m_major_version { 0 };

    WaitQueue m_irq_queue;
};
}
