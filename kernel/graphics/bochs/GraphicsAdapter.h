/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Types.h>
#include <kernel/bus/pci/DeviceController.h>
#include <kernel/graphics/console/GenericFramebufferConsole.h>
#include <kernel/graphics/FramebufferDevice.h>
#include <kernel/graphics/GraphicsDevice.h>
#include <kernel/memory/TypedMapping.h>
#include <kernel/PhysicalAddress.h>

namespace Kernel {

class GraphicsManagement;
struct BochsDisplayMMIORegisters;

class BochsGraphicsAdapter final : public GraphicsDevice
    , public PCI::DeviceController {
    BASE_MAKE_ETERNAL
    friend class GraphicsManagement;

public:
    static NonnullRefPtr<BochsGraphicsAdapter> initialize(PCI::Address);
    virtual ~BochsGraphicsAdapter() = default;
    virtual bool framebuffer_devices_initialized() const override { return !m_framebuffer_device.is_null(); }

    virtual bool modesetting_capable() const override { return true; }
    virtual bool double_framebuffering_capable() const override { return true; }

private:

    virtual bool try_to_set_resolution(size_t output_port_index, size_t width, size_t height) override;
    virtual bool set_y_offset(size_t output_port_index, size_t y) override;

    virtual void initialize_framebuffer_devices() override;
    virtual Type type() const override;

    virtual void enable_consoles() override;
    virtual void disable_consoles() override;

    explicit BochsGraphicsAdapter(PCI::Address);

    void set_safe_resolution();
    void unblank();

    bool validate_setup_resolution(size_t width, size_t height);
    u32 find_framebuffer_address();
    void set_resolution_registers(size_t width, size_t height);
    void set_resolution_registers_via_io(size_t width, size_t height);
    bool validate_setup_resolution_with_io(size_t width, size_t height);
    void set_y_offset(size_t);

    PhysicalAddress m_mmio_registers;
    Memory::TypedMapping<BochsDisplayMMIORegisters volatile> m_registers;
    RefPtr<FramebufferDevice> m_framebuffer_device;
    RefPtr<Graphics::GenericFramebufferConsole> m_framebuffer_console;
    SpinLock<u8> m_console_mode_switch_lock;
    bool m_console_enabled { false };
    bool m_io_required { false };
};

}
