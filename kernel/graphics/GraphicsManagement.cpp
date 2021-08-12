/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <kernel/bus/pci/IDs.h>
#include <kernel/CommandLine.h>
#include <kernel/graphics/bochs/GraphicsAdapter.h>
#include <kernel/graphics/GraphicsManagement.h>
#include <kernel/graphics/kntel/NativeGraphicsAdapter.h>
#include <kernel/graphics/VGACompatibleAdapter.h>
#include <kernel/graphics/VirtIOGPU/GraphicsAdapter.h>
#include <kernel/IO.h>
#include <kernel/memory/AnonymousVMObject.h>
#include <kernel/Multiboot.h>
#include <kernel/Sections.h>

namespace Kernel {

static Singleton<GraphicsManagement> s_the;

GraphicsManagement& GraphicsManagement::the()
{
    return *s_the;
}

bool GraphicsManagement::is_initialized()
{
    return s_the.is_initialized();
}

UNMAP_AFTER_INIT GraphicsManagement::GraphicsManagement()
    : m_framebuffer_devices_allowed(!kernel_command_line().is_no_framebuffer_devices_mode())
{
}

void GraphicsManagement::deactivate_graphical_mode()
{
    for (auto& graphics_device : m_graphics_devices) {
        graphics_device.enable_consoles();
    }
}
void GraphicsManagement::activate_graphical_mode()
{
    for (auto& graphics_device : m_graphics_devices) {
        graphics_device.disable_consoles();
    }
}

static inline bool is_vga_compatible_pci_device(PCI::Address address)
{
    auto is_display_controller_vga_compatible = PCI::get_class(address) == 0x3 && PCI::get_subclass(address) == 0x0;
    auto is_general_pci_vga_compatible = PCI::get_class(address) == 0x0 && PCI::get_subclass(address) == 0x1;
    return is_display_controller_vga_compatible || is_general_pci_vga_compatible;
}

static inline bool is_display_controller_pci_device(PCI::Address address)
{
    return PCI::get_class(address) == 0x3;
}

UNMAP_AFTER_INIT bool GraphicsManagement::determine_and_initialize_graphics_device(const PCI::Address& address, PCI::ID id)
{
    VERIFY(is_vga_compatible_pci_device(address) || is_display_controller_pci_device(address));
    auto add_and_configure_adapter = [&](GraphicsDevice& graphics_device) {
        m_graphics_devices.append(graphics_device);
        if (!m_framebuffer_devices_allowed) {
            graphics_device.enable_consoles();
            return;
        }
        graphics_device.initialize_framebuffer_devices();
    };

    RefPtr<GraphicsDevice> adapter;
    switch (id.vendor_id) {
    case PCI::VendorID::QEMUOld:
        if (id.device_id == 0x1111)
            adapter = BochsGraphicsAdapter::initialize(address);
        break;
    case PCI::VendorID::VirtualBox:
        if (id.device_id == 0xbeef)
            adapter = BochsGraphicsAdapter::initialize(address);
        break;
    case PCI::VendorID::Intel:
        adapter = IntelNativeGraphicsAdapter::initialize(address);
        break;
    case PCI::VendorID::VirtIO:
        dmesgln("Graphics: Using VirtIO console");
        adapter = Graphics::VirtIOGPU::GraphicsAdapter::initialize(address);
        break;
    default:
        if (!is_vga_compatible_pci_device(address))
            break;

        if (!m_vga_adapter && PCI::is_io_space_enabled(address)) {
            if (multiboot_framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
                dmesgln("Graphics: Using a preset resolution from the bootloader");
                adapter = VGACompatibleAdapter::initialize_with_preset_resolution(address,
                    multiboot_framebuffer_addr,
                    multiboot_framebuffer_width,
                    multiboot_framebuffer_height,
                    multiboot_framebuffer_pitch);
            }
        } else {
            dmesgln("Graphics: Using a VGA compatible generic adapter");
            adapter = VGACompatibleAdapter::initialize(address);
        }
        break;
    }
    if (!adapter)
        return false;
    add_and_configure_adapter(*adapter);


    if (!m_vga_adapter && PCI::is_io_space_enabled(address) && adapter->type() == GraphicsDevice::Type::VGACompatible) {
        dbgln("Graphics adapter @ {} is operating in VGA mode", address);
        m_vga_adapter = adapter;
    }
    return true;
}

UNMAP_AFTER_INIT bool GraphicsManagement::initialize()
{

    if (kernel_command_line().is_no_framebuffer_devices_mode()) {
        dbgln("Forcing no initialization of framebuffer devices");
    }

    PCI::enumerate([&](const PCI::Address& address, PCI::ID id) {

        if (!is_vga_compatible_pci_device(address) && !is_display_controller_pci_device(address))
            return;
        determine_and_initialize_graphics_device(address, id);
    });

    if (m_graphics_devices.is_empty()) {
        dbgln("No graphics adapter was initialized.");
        return false;
    }
    return true;
}

bool GraphicsManagement::framebuffer_devices_exist() const
{
    for (auto& graphics_device : m_graphics_devices) {
        if (graphics_device.framebuffer_devices_initialized())
            return true;
    }
    return false;
}
}
