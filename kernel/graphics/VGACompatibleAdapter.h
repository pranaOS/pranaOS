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
#include <kernel/graphics/console/Console.h>
#include <kernel/graphics/FramebufferDevice.h>
#include <kernel/graphics/GraphicsDevice.h>
#include <kernel/PhysicalAddress.h>

namespace Kernel {

class VGACompatibleAdapter : public GraphicsDevice
    , public PCI::DeviceController {
    BASE_MAKE_ETERNAL
public:
    static NonnullRefPtr<VGACompatibleAdapter> initialize_with_preset_resolution(PCI::Address, PhysicalAddress, size_t framebuffer_width, size_t framebuffer_height, size_t framebuffer_pitch);
    static NonnullRefPtr<VGACompatibleAdapter> initialize(PCI::Address);

    virtual bool framebuffer_devices_initialized() const override { return !m_framebuffer_device.is_null(); }

    virtual bool modesetting_capable() const override { return false; }
    virtual bool double_framebuffering_capable() const override { return false; }

    virtual bool try_to_set_resolution(size_t output_port_index, size_t width, size_t height) override;
    virtual bool set_y_offset(size_t output_port_index, size_t y) override;

protected:
    explicit VGACompatibleAdapter(PCI::Address);

private:
    VGACompatibleAdapter(PCI::Address, PhysicalAddress, size_t framebuffer_width, size_t framebuffer_height, size_t framebuffer_pitch);

    virtual void initialize_framebuffer_devices() override;
    virtual Type type() const override { return Type::VGACompatible; }

    virtual void enable_consoles() override;
    virtual void disable_consoles() override;

protected:
    PhysicalAddress m_framebuffer_address;
    size_t m_framebuffer_width { 0 };
    size_t m_framebuffer_height { 0 };
    size_t m_framebuffer_pitch { 0 };

    RefPtr<FramebufferDevice> m_framebuffer_device;
    RefPtr<Graphics::Console> m_framebuffer_console;
};

}
